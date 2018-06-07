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

using addr_t = uint32_t;

using VType = variant<int64_t, uint64_t, double, bool, char, addr_t>;

struct ValueType;
struct ReferenceType;

template <typename T>
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
  VType Value;

  ReferenceType() { type = NIL; }

  ReferenceType(ValueType value) : type{value.type}, Value{value.Value} {}

  ReferenceType(type_t type, VType value) : type{type}, Value{value} {}

  Result<ValueType> Unbox();
};

template <typename T>
class Result {
  result_t m_rtype;
  variant<Error, T> m_result;

 public:
  Result(result_t rtype, variant<Error, T> result)
      : m_rtype{rtype}, m_result{result} {}

  result_t ResultType() { return m_rtype; };
  variant<Error, T> Read() { return m_result; };

  template <typename R = T>
  Result<R> Apply(function<Result<R>(T)> f);

  template <typename R = T>
  Result<R> ApplyEither(function<Result<R>(T)> ok,
                        function<Result<R>(T)> error);

  template <typename U = T, typename R = T>
  Result<R> ZipWith(function<Result<R>(T, U)> f, Result<U> other);
};

template <typename T>
Result<T> ReturnError(ErrorCode error_code, string msg) {
  return Result<T>{ERROR, Error{error_code, msg}};
}

template <typename T>
Result<T> ReturnOk(T t) {
  return Result<T>{OK, t};
}

template <typename T>
template <typename R>
Result<R> Result<T>::Apply(function<Result<R>(T)> f) {
  switch (this->ResultType()) {
    case ERROR:
      return ReturnError<R>(INVALID_ARGUMENT, "");
    case OK:
      return f(get<T>(this->Read()));
    default:
      return ReturnError<R>(INVALID_ARGUMENT, "");
  }
}

template <typename T>
template <typename R>
Result<R> Result<T>::ApplyEither(function<Result<R>(T)> ok,
                                 function<Result<R>(T)> error) {
  switch (this->ResultType()) {
    case ERROR:
      return error();
    case OK:
      return ok(get<T>(this->Read()));
    default:
      return ReturnError<R>(INVALID_ARGUMENT, "");
  }
}

template <typename T, typename U, typename R>
struct ZipWithFunctor {
  T r1;
  function<Result<R>(T, U)> f;
  ZipWithFunctor(function<Result<R>(T, U)> f, T r1) : r1{r1}, f{f} {}

  Result<R> operator()(U r2) { return f(r1, r2); }
};

template <typename T>
template <typename U, typename R>
Result<R> Result<T>::ZipWith(function<Result<R>(T, U)> f, Result<U> other) {
  switch (this->ResultType()) {
    case ERROR:
      return ReturnError<R>(INVALID_ARGUMENT, "");
    case OK:
      return other.template Apply<R>(
          ZipWithFunctor<T, U, R>{f, get<T>(this->Read())});
    default:
      return ReturnError<R>(INVALID_ARGUMENT, "");
  }
}

template <typename T1, typename T2>
Result<ValueType> ArithmeticInner(ArithmeticOperator op, T1 v1, T2 v2,
                                  type_t v1_type, type_t v2_type) {
  switch (op) {
    case ADDOP:
      return ReturnOk<ValueType>(ValueType{max(v1_type, v2_type), v1 + v2});
    case SUBOP:
      return ReturnOk<ValueType>(ValueType{max(v1_type, v2_type), v1 - v2});
    case MULOP:
      return ReturnOk<ValueType>(ValueType{max(v1_type, v2_type), v1 * v2});
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
  Result<ValueType> operator()(ValueType v1, ValueType v2) {
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

auto ArithmeticAdd = ArithmeticFunctor{ADDOP};
auto ArithmeticSub = ArithmeticFunctor{SUBOP};
auto ArithmeticMul = ArithmeticFunctor{MULOP};

}  // namespace dlvm
