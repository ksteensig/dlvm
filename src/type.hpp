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

// Left type and Right type
template <typename E, typename T>
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
typedef enum { LEFT, RIGHT } result_t;

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

  Result<Error, ValueType> Unbox();
};

template <typename E, typename T>
class Result {
  result_t m_rtype;
  variant<E, T> m_result;

 public:
  Result(result_t rtype, variant<E, T> result)
      : m_rtype{rtype}, m_result{result} {}

  result_t ResultType() { return m_rtype; };
  variant<E, T> Read() { return m_result; };

  template <typename R = T>
  Result<E, R> RightMap(function<Result<E, R>(T)> f);

  template <typename R = E>
  Result<R, E> LeftMap(function<Result<R, T>(E)> f);

  template <typename U = E, typename R = T>
  Result<U, R> Then(function<Result<U, R>(void)> f);

  template <typename U = T, typename R = T>
  Result<E, R> RightZip(function<Result<E, R>(T, U)> f, Result<E, U> other);

  template <typename U = E, typename R = E>
  Result<R, T> LeftZip(function<Result<R, T>(E, U)> f, Result<E, U> other);
};

template <typename E, typename T>
Result<E, T> ReturnLeft(E e) {
  return Result<E, T>{LEFT, e};
}

template <typename E, typename T>
Result<E, T> ReturnRight(T t) {
  return Result<E, T>{RIGHT, t};
}

template <typename T>
Result<Error, T> ReturnError(ErrorCode error_code, string msg) {
  return ReturnLeft<Error, T>(Error{error_code, msg});
}

template <typename T>
Result<Error, T> ReturnOk(T t) {
  return ReturnRight<Error, T>(t);
}

template <typename E, typename T>
template <typename R>
Result<E, R> Result<E, T>::RightMap(function<Result<E, R>(T)> f) {
  switch (this->ResultType()) {
    case RIGHT:
      return f(get<T>(this->Read()));
    case LEFT:
    default:
      return ReturnLeft<E, R>(get<0>(this->Read()));
  }
}

template <typename E, typename T>
template <typename R>
Result<R, E> Result<E, T>::LeftMap(function<Result<R, T>(E)> f) {
  switch (this->ResultType()) {
    case LEFT:
      return f(get<E>(this->Read()));
    case RIGHT:
    default:
      return ReturnLeft<R, T>(get<1>(this->Read()));
  }
}

template <typename E, typename T>
template <typename U, typename R>
Result<U, R> Result<E, T>::Then(function<Result<U, R>(void)> f) {
  return f();
}

template <typename E, typename T, typename U, typename R>
struct ZipRightFunctor {
  T r1;
  function<Result<E, R>(T, U)> f;
  ZipRightFunctor(function<Result<E, R>(T, U)> f, T r1) : r1{r1}, f{f} {}

  Result<E, R> operator()(U r2) { return f(r1, r2); }
};

template <typename E, typename T, typename U, typename R>
struct ZipLeftFunctor {
  E r1;
  function<Result<R, T>(E, U)> f;
  ZipLeftFunctor(function<Result<E, R>(T, U)> f, T r1) : r1{r1}, f{f} {}

  Result<R, E> operator()(U r2) { return f(r1, r2); }
};

template <typename E, typename T>
template <typename U, typename R>
Result<E, R> Result<E, T>::RightZip(function<Result<E, R>(T, U)> f,
                                    Result<E, U> other) {
  switch (this->ResultType()) {
    case RIGHT:
      return other.template RightMap<R>(
          ZipRightFunctor<E, T, U, R>{f, get<T>(this->Read())});
    case LEFT:
    default:
      return *this;
  }
}

template <typename E, typename T>
template <typename U, typename R>
Result<R, T> Result<E, T>::LeftZip(function<Result<R, T>(E, U)> f,
                                   Result<E, U> other) {
  switch (this->ResultType()) {
    case LEFT:
      return other.template LeftMap<R>(
          ZipLeftFunctor<T, E, U, R>{f, get<E>(this->Read())});
    case RIGHT:
    default:
      return *this;
  }
}

template <typename T1, typename T2>
Result<Error, ValueType> ArithmeticInner(ArithmeticOperator op, T1 v1, T2 v2,
                                         type_t v1_type, type_t v2_type) {
  switch (op) {
    case ADDOP:
      return ReturnOk<>(ValueType{max(v1_type, v2_type), v1 + v2});
    case SUBOP:
      return ReturnOk<>(ValueType{max(v1_type, v2_type), v1 - v2});
    case MULOP:
      return ReturnOk<>(ValueType{max(v1_type, v2_type), v1 * v2});
    default:
      return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }
}

template <typename T>
Result<Error, ValueType> ArithmeticOuter(ArithmeticOperator op, T v1,
                                         ValueType v2, type_t v1_type) {
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
  Result<Error, ValueType> operator()(ValueType v1, ValueType v2) {
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
