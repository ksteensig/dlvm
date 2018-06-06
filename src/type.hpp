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

using namespace dlvm;
using namespace std;
using namespace std::placeholders;

using vaddr_t = uint64_t;
using addr_t = uint32_t;

using array_t = uint32_t;  // length of an array
using VType = variant<int64_t, uint64_t, double, bool, char, addr_t>;
using RType = variant<array_t, VType>;

struct ValueType;
struct ReferenceType;

template <typename T1, typename T2 = T1, typename R = T1>
class Result;

typedef enum type_e {
  NIL,
  UINTEGER,
  INTEGER,
  FLOAT,
  BOOL,
  STRING,
  ARRAY,
  PTR,
  NATIVE_PTR,
  CLOSURE,
} type_t;

typedef enum { ADDOP, SUBOP, MULOP } ArithmeticOperator;
typedef enum { ERROR, OK } result_t;

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
  RType Value;

  ReferenceType() { type = NIL; }

  ReferenceType(ValueType value) : type{value.type}, Value{value.Value} {}

  ReferenceType(type_t type, RType value) : type{type}, Value{value} {}

  Result<ValueType> Unbox();
};

template <typename T1, typename T2, typename R>
class Result {
  result_t m_rtype;
  variant<Error, T1> m_result;

 public:
  Result(result_t rtype, variant<Error, T1> result)
      : m_rtype{rtype}, m_result{result} {}

  result_t ResultType() { return m_rtype; };
  variant<Error, T1> Read() { return m_result; };

  Result<R> Apply(function<Result<R>(T1)> f);
  Result<R> ZipWith(function<Result<R>(T1, T2)> f, Result<T2> other);
};

template <typename T>
Result<T> LiftError(ErrorCode error_code, string msg) {
  return Result<T>{ERROR, Error{error_code, msg}};
}

template <typename T>
Result<T> LiftType(T t) {
  return Result<T>{OK, t};
}

template <typename T1, typename T2, typename R>
Result<R> Result<T1, T2, R>::Apply(function<Result<R>(T1)> f) {
  switch (this->ResultType()) {
    case ERROR:
      return *this;
    case OK:
      return f(get<T2>(this->Read()));
    default:
      return LiftError<R>(INVALID_ARGUMENT, "");
  }
}

template <typename T1, typename T2, typename R>
struct ZipWithFunctor {
  T1 r1;
  function<Result<R>(T1, T2)> f;
  ZipWithFunctor(function<Result<R>(T1, T2)> f, T1 r1) : r1{r1}, f{f} {}

  Result<R> operator()(T2 r2) { return f(r1, r2); }
};

template <typename T1, typename T2, typename R>
Result<R> Result<T1, T2, R>::ZipWith(function<Result<R>(T1, T2)> f,
                                     Result<T2> other) {
  switch (this->ResultType()) {
    case ERROR:
      return *this;
    case OK:
      return other.Apply(ZipWithFunctor<T1, T2, R>{f, get<T1>(this->Read())});
    default:
      return LiftError<R>(INVALID_ARGUMENT, "");
  }
}

template <typename T1, typename T2>
Result<ValueType> ArithmeticApply(ArithmeticOperator op, T1 v1, T2 v2,
                                  type_t v1_type, type_t v2_type) {
  switch (op) {
    case ADDOP:
      return LiftType<ValueType>(ValueType{max(v1_type, v2_type), v1 + v2});
    case SUBOP:
      return LiftType<ValueType>(ValueType{max(v1_type, v2_type), v1 - v2});
    case MULOP:
      return LiftType<ValueType>(ValueType{max(v1_type, v2_type), v1 * v2});
    default:
      return LiftError<ValueType>(INVALID_ARGUMENT, "");
  }
}

template <typename T>
Result<ValueType> Arithmetic(ArithmeticOperator op, T v1, ValueType v2,
                             type_t v1_type) {
  switch (v2.type) {
    case INTEGER:
      return ArithmeticApply<T, int64_t>(op, v1, get<int64_t>(v2.Value),
                                         v1_type, INTEGER);
    case UINTEGER:
      return ArithmeticApply<T, uint64_t>(op, v1, get<uint64_t>(v2.Value),
                                          v1_type, UINTEGER);
    case FLOAT:
      return ArithmeticApply<T, double>(op, v1, get<double>(v2.Value), v1_type,
                                        FLOAT);
    default:
      return LiftError<ValueType>(INVALID_ARGUMENT, "");
  }
}

struct ArithmeticFunctor {
  ArithmeticOperator op;
  ArithmeticFunctor(ArithmeticOperator op) : op{op} {}
  Result<ValueType> operator()(ValueType v1, ValueType v2) {
    switch (v1.type) {
      case INTEGER:
        return Arithmetic<int64_t>(op, get<int64_t>(v1.Value), v2, INTEGER);
      case UINTEGER:
        return Arithmetic<uint64_t>(op, get<uint64_t>(v1.Value), v2, UINTEGER);
      case FLOAT:
        return Arithmetic<double>(op, get<double>(v1.Value), v2, FLOAT);
      default:
        return LiftError<ValueType>(INVALID_ARGUMENT, "");
    }
  }
};

auto ArithmeticAdd = ArithmeticFunctor{ADDOP};
auto ArithmeticSub = ArithmeticFunctor{SUBOP};
auto ArithmeticMul = ArithmeticFunctor{MULOP};

}  // namespace dlvm
