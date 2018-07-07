#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "error.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;
using namespace std::placeholders;

using addr_t = uint32_t;
using VType = variant<int64_t, uint64_t, double, bool, char, addr_t>;
using NativeFunc = void* (*)(void*);

struct ValueType;
struct ReferenceType;

// Left type and Right type
template <typename T>
class Result;

typedef enum type_e {
  NIL,
  UINTEGER,
  INTEGER,
  FLOAT,
  BOOL,
  PTR,
  NATIVE_PTR,
  CLOSURE,
} type_t;

typedef enum { ADDOP, SUBOP, MULOP, DIVOP } ArithmeticOperator;

struct ValueType {
  type_t type;
  VType Value;

  ValueType() { type = NIL; }

  ValueType(type_t type, VType value) : type{type}, Value{value} {}

  ReferenceType Box();
};

struct ReferenceType {
  type_t type;
  bool Marked = false;
  VType Value;

  ReferenceType() { type = NIL; }

  ReferenceType(ValueType value) : type{value.type}, Value{value.Value} {}

  ReferenceType(type_t type, VType value) : type{type}, Value{value} {}

  Result<ValueType> Unbox();
};

template <typename T>
class Result {
  bool is_error;
  variant<Error, T> m_result;

 public:
  Result(bool is_error, variant<Error, T> result)
      : is_error{is_error}, m_result{result} {}

  bool IsError() { return is_error; };
  variant<Error, T> Read() { return m_result; };

  template <typename R, typename F>
  Result<R> MapOk(F f);

  template <typename R, typename U, typename F>
  Result<R> MapOk(F f, Result<U> other);

  template <typename F>
  Result<T> MapError(F f);

  template <typename F, typename G>
  Result<T> IfErrorElseOk(F lf, G rf);

  Result<T> OnError();

  T fromOk();
};

template <typename T>
Result<T> ReturnError(Error e) {
  return Result<T>{true, e};
}

template <typename T>
Result<T> ReturnError(ErrorCode error_code, string msg) {
  return Result<T>{true, Error{error_code, msg}};
}

template <typename T>
Result<T> ReturnOk(T t) {
  return Result<T>{false, t};
}

template <typename T>
template <typename R, typename F>
Result<R> Result<T>::MapOk(F f) {
  is_convertible<F, function<Result<R>(T)>>{};
  if (this->IsError()) {
    return ReturnError<R>(get<Error>(this->Read()));
  } else {
    return f(get<T>(this->Read()));
  }
}

template <typename T>
template <typename R, typename U, typename F>
Result<R> Result<T>::MapOk(F f, Result<U> other) {
  is_convertible<F, function<Result<R>(T, U)>>{};
  if (this->IsError()) {
    return ReturnError<R>(get<Error>(this->Read()));
  } else {
    auto fu = [f, this](U u) { return f(get<T>(this->Read()), u); };
    return other.template MapOk<R>(fu);
  }
}

template <typename T>
template <typename F>
Result<T> Result<T>::MapError(F f) {
  is_convertible<F, function<Result<T>(Error)>>{};
  if (!this->IsError()) {
    return *this;
  } else {
    return f(get<Error>(this->Read()));
  }
}

template <typename T>
template <typename F, typename G>
Result<T> Result<T>::IfErrorElseOk(F lf, G rf) {
  is_convertible<F, function<Result<T>(Error)>>{};
  is_convertible<G, function<Result<T>(T)>>{};
  if (this->IsError()) {
    return lf(get<Error>(this->Read()));
  } else {
    return rf(get<T>(this->Read()));
  }
}

template <typename T, typename U = T, typename R = T, typename F, typename G>
function<Result<R>(U)> ComposeOk(F first, G second) {
  is_convertible<F, function<Result<T>(U)>>{};
  is_convertible<G, function<Result<R>(T)>>{};
  return [first, second](U u) { return first(u).template MapOk<R>(second); };
}

template <typename T>
Result<T> Result<T>::OnError() {
  auto err = [](Error e) {
    cout << "Typecode " << e.ErrCode << ": " << e.Message << endl;
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
      return ArithmeticInner<T, int64_t>(op, v1, get<int64_t>(v2.Value),
                                         v1_type, INTEGER);
    case UINTEGER:
      return ArithmeticInner<T, uint64_t>(op, v1, get<uint64_t>(v2.Value),
                                          v1_type, UINTEGER);
    case FLOAT:
      return ArithmeticInner<T, double>(op, v1, get<double>(v2.Value), v1_type,
                                        FLOAT);
    default:
      return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }
}

struct ArithmeticFunctor {
  ArithmeticOperator op;
  ArithmeticFunctor(ArithmeticOperator op) : op{op} {}
  Result<ValueType> operator()(ValueType v1, ValueType v2) const {
    switch (v1.type) {
      case INTEGER:
        return ArithmeticOuter<int64_t>(op, get<int64_t>(v1.Value), v2,
                                        INTEGER);
      case UINTEGER:
        return ArithmeticOuter<uint64_t>(op, get<uint64_t>(v1.Value), v2,
                                         UINTEGER);
      case FLOAT:
        return ArithmeticOuter<double>(op, get<double>(v1.Value), v2, FLOAT);
      default:
        return ReturnError<ValueType>(INVALID_ARGUMENT, "");
    }
  }
};

}  // namespace dlvm
