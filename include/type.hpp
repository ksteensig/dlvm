#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "error.hpp"

namespace dlvm {

using std::function;
using std::get;
using std::max;
using std::variant;
using namespace std::placeholders;

using addr_t = uint32_t;

// Value type declared as a variant, this is used by the ValueType struct
using VType = variant<int64_t, uint64_t, double, bool, char, addr_t>;

// DLVM type to represent a function pointer to native code
using NativeFunc = void* (*)(void*);

struct ValueType;
struct ReferenceType;

template <typename T>
class Result;

// type tags
typedef enum type_e {
  NIL,       // Pretty much the NULL of DLVM
  UINTEGER,  // 64bit unsigned integer
  INTEGER,   // 64bit signed integer
  FLOAT,     // double precision floating point
  BOOL,
  PTR,         // ptr to managed heap memory
  NATIVE_PTR,  // ptr to native memory
  ARRAY,
  CLOSURE,  // declared for future use
} type_t;

// arithmetic operators
// TODO: (mod) and probably also another type for boolean logic
typedef enum { ADDOP, SUBOP, MULOP, DIVOP, MODOP } ArithmeticOperator;

typedef enum {
  ANDOP,
  OROP,
  XOROP,
  NOTOP,
  LTOP,
  LEOP,
  EQOP,
  GEOP,
  GTOP
} BooleanOperator;

// Representation of value types in DLVM
// They are also used internally with the monadicish interface,
// because they are pushed to and popped from the DLVM stack
struct alignas(16) ValueType {
  type_t type;  // type tag
  VType Value;  // actual value

  ValueType() { type = NIL; }

  ValueType(type_t type, VType value) : type{type}, Value{value} {}

  ReferenceType Box();
};

// ReferenceType can be unboxed into a ValueType and put on the stack
// When boxing a ValueType it can be put in the heap
struct alignas(16) ReferenceType {
  type_t type;          // type tag
  bool Marked = false;  // used by garbage collector to determine if to remove
  VType Value;          // actual value

  ReferenceType() { type = NIL; }

  ReferenceType(ValueType value) : type{value.type}, Value{value.Value} {}

  ReferenceType(type_t type, VType value) : type{type}, Value{value} {}

  Result<ValueType> Unbox();
};

// Monadicish interface to do error handling
// All errors in DLVM can in practice be handled in OnError
// IsError and Read are used internally by DLVM
template <typename T>
class Result {
  bool is_error;
  variant<Error, T> m_result;

 public:
  Result(bool is_error, variant<Error, T> result)
      : is_error{is_error}, m_result{result} {}

  bool IsError() { return is_error; };
  variant<Error, T> Read() { return m_result; };

  template <typename R = T>
  // MapOk works as a map function if Result is not an error, else it does
  // nothing
  Result<R> MapOk(function<Result<R>(T)> f);

  // MapError works as a map function if Result is an error, else it does
  // nothing
  Result<T> MapError(function<Result<T>(Error e)> f);

  Result<T> IfErrorElseOk(function<Result<T>(Error)> lf,
                          function<Result<T>(T)> rf);

  template <typename U = T, typename R = T>
  // is almost like bind for monads
  Result<R> AggregateOk(function<Result<R>(T, U)> f, Result<U> other);

  // DLVM error handling method
  Result<T> OnError();

  // returns internally stored object of type T if Result is not an error
  // invokes error handling if it is an error
  T fromOk();

  std::pair<bool, std::variant<Error, T>> Apply();
};

template <typename T>
Result<T> ReturnError(Error e) {
  return Result<T>{true, e};
}

template <typename T>
Result<T> ReturnError(ErrorCode error_code, std::string msg) {
  return Result<T>{true, Error{error_code, msg}};
}

template <typename T>
Result<T> ReturnOk(T t) {
  return Result<T>{false, t};
}

template <typename T>
template <typename R>
Result<R> Result<T>::MapOk(function<Result<R>(T)> f) {
  if (this->IsError()) {
    return ReturnError<R>(get<Error>(this->Read()));
  } else {
    return f(get<T>(this->Read()));
  }
}

template <typename T>
Result<T> Result<T>::MapError(function<Result<T>(Error)> f) {
  if (!this->IsError()) {
    return *this;
  } else {
    return f(get<Error>(this->Read()));
  }
}

template <typename T>
Result<T> Result<T>::IfErrorElseOk(function<Result<T>(Error)> lf,
                                   function<Result<T>(T)> rf) {
  if (this->IsError()) {
    return lf(get<Error>(this->Read()));
  } else {
    return rf(get<T>(this->Read()));
  }
}

template <typename T, typename U = T, typename R = T>
function<Result<R>(U)> ComposeOk(function<Result<T>(U)> first,
                                 function<Result<R>(T)> second) {
  return [first, second](U u) { return first(u).MapOk(second); };
}

template <typename T>
template <typename U, typename R>
Result<R> Result<T>::AggregateOk(function<Result<R>(T, U)> f, Result<U> other) {
  if (this->IsError()) {
    return ReturnError<R>(get<Error>(this->Read()));
  } else {
    function<Result<R>(U)> fu = [f, this](U u) {
      return f(get<T>(this->Read()), u);
    };
    return other.MapOk(fu);
  }
}

template <typename T>
Result<T> Result<T>::OnError() {
  auto err = [](Error e) {
    std::cout << "Typecode " << e.ErrCode << ": " << e.Message << std::endl;
    exit(1);
    return ReturnError<T>(e);
  };

  return this->MapError(err);
}

template <typename T>
T Result<T>::fromOk() {
  return get<T>(this->OnError().Read());
}

template <typename T1, typename T2>
Result<ValueType> ArithmeticInner(ArithmeticOperator op, T1 v1, T2 v2,
                                  type_t v1_type, type_t v2_type) {
  switch (op) {
    case ADDOP:
      return ReturnOk<>(ValueType{max(v1_type, v2_type), v1 + v2});
    case SUBOP:
      return ReturnOk<>(ValueType{max(v1_type, v2_type), v1 - v2});
    case MULOP:
      return ReturnOk<>(ValueType{max(v1_type, v2_type), v1 * v2});
    case DIVOP:
      if (v2 == 0) {
        return ReturnError<ValueType>(DIVISION_BY_ZERO, "");
      } else {
        return ReturnOk<>(ValueType{max(v1_type, v2_type), v1 / v2});
      }
    default:
      return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }
}

template <typename T>
Result<ValueType> ArithmeticOuter(ArithmeticOperator op, T v1, ValueType v2,
                                  type_t v1_type) {
  switch (v2.type) {
    case INTEGER:
      return ArithmeticInner<T, int64_t>(op, v1, std::get<int64_t>(v2.Value),
                                         v1_type, INTEGER);
    case UINTEGER:
      return ArithmeticInner<T, uint64_t>(op, v1, std::get<uint64_t>(v2.Value),
                                          v1_type, UINTEGER);
    case FLOAT:
      return ArithmeticInner<T, double>(op, v1, std::get<double>(v2.Value),
                                        v1_type, FLOAT);
    default:
      return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }
}

struct ArithmeticFunctor {
  ArithmeticOperator op;
  ArithmeticFunctor(ArithmeticOperator op) : op{op} {}
  Result<ValueType> operator()(ValueType v1, ValueType v2) {
    switch (v1.type) {
      case INTEGER:
        return ArithmeticOuter<int64_t>(op, std::get<int64_t>(v1.Value), v2,
                                        INTEGER);
      case UINTEGER:
        return ArithmeticOuter<uint64_t>(op, std::get<uint64_t>(v1.Value), v2,
                                         UINTEGER);
      case FLOAT:
        return ArithmeticOuter<double>(op, std::get<double>(v1.Value), v2,
                                       FLOAT);
      default:
        return ReturnError<ValueType>(INVALID_ARGUMENT, "");
    }
  }
};

/*
case LTOP:
case LEOP:
case EQOP:
case GEOP:
case GTOP:
*/

/*

template <typename T1, typename T2>
Result<ValueType> BooleanInner(BooleanOperator op, T1 v1, T2 v2, type_t v1_type,
                               type_t v2_type) {
  switch (op) {
    case ANDOP:
      return ReturnOk<>(ValueType{BOOL, v1 && v2});
    case OROP:
      return ReturnOk<>(ValueType{BOOL, v1 || v2});
    case XOROP:
      // return ReturnOk<>(ValueType{BOOL, v1 v2});
    case NOTOP:
    default:
      return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }
}


template <typename T>
Result<ValueType> BooleanOuter(BooleanOperator op, T v1, ValueType v2,
                               type_t v1_type) {
  switch (v2.type) {
    case BOOL:
      return BooleanInner<T, int64_t>(op, v1, std::get<int64_t>(v2.Value),
    default:
      return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }
}

struct BooleanFunctor {
  BooleanOperator op;
  BooleanFunctor(BooleanOperator op) : op{op} {}
  Result<ValueType> operator()(ValueType v1, ValueType v2) {
    switch (v1.type) {
      case INTEGER:
        return BooleanOuter<int64_t>(op, std::get<int64_t>(v1.Value), v2,
                                     INTEGER);
      case UINTEGER:
        return BooleanOuter<uint64_t>(op, std::get<uint64_t>(v1.Value), v2,
                                      UINTEGER);
      case FLOAT:
        return BooleanOuter<double>(op, std::get<double>(v1.Value), v2, FLOAT);
      default:
        return ReturnError<ValueType>(INVALID_ARGUMENT, "");
    }
  }
};
*/

}  // namespace dlvm
