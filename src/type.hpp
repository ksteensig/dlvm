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

using array_t = uint32_t;
using VType = variant<int64_t, uint64_t, double, bool, char, addr_t>;
using RType = variant<array_t, VType>;

struct ValueType;
struct ReferenceType;

template <typename Type>
struct Result;

typedef enum type_e {
  NIL,
  INTEGER,
  UINTEGER,
  FLOAT,
  BOOL,
  STRING,
  ARRAY,
  REFERENCE,
  STRUCT,
  CLOSURE
} type_t;

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

typedef enum { ERROR, OK } result_t;

template <typename Type>
struct Result {
  result_t rtype;
  variant<Error, Type> result;

  Result(result_t rtype, variant<Error, Type> result)
      : rtype{rtype}, result{result} {}

  bool isError() { return rtype == ERROR; }
  bool isType() { return rtype == OK; }
};

template <typename Functor, typename Type>
Result<Type> MapResult(function<Result<Type>(Result<Type>)> f, Result<Type> r) {
  switch (r.rtype) {
    case ERROR:
      return r;
    case OK:
      return LiftType(f(r));
  }
}

template <typename Type>
Result<Type> MapResult2(function<Result<Type>(Result<Type>, Result<Type>)> f,
                        Result<Type> r1, Result<Type> r2) {
  switch (r1.rtype) {
    case ERROR:
      return r1;
    case OK:
      return LiftResult(bind(f, get<Type>(r1), _2), r2);
  }
}

template <typename Type>
Result<Type> LiftError(ErrorCode error_code, string msg) {
  return Result<Type>{ERROR, Error{error_code, msg}};
}

template <typename Type>
Result<Type> LiftType(Type t) {
  return Result<Type>{OK, t};
}

typedef enum { ADDOP, SUBOP, MULOP } ArithmeticOperator;

template <typename T1, typename T2>
Result<VType> ArithmeticApply(ArithmeticOperator op, T1 v1, T2 v2) {
  switch (op) {
    case ADDOP:
      return LiftType<VType>(v1 + v2);
    default:
      return LiftError<VType>(INVALID_ARGUMENT, "");
  }
}

template <typename T>
Result<VType> Arithmetic(ArithmeticOperator op, T v1, ValueType v2) {
  switch (v2.type) {
    case INTEGER:
      return ArithmeticApply<T, int64_t>(op, v1, get<int64_t>(v2.Value));
    case UINTEGER:
      return ArithmeticApply<T, uint64_t>(op, v1, get<uint64_t>(v2.Value));
    case FLOAT:
      return ArithmeticApply<T, double>(op, v1, get<double>(v2.Value));
    default:
      return LiftError<VType>(INVALID_ARGUMENT, "");
  }
}

Result<VType> Arithmetic2(ArithmeticOperator op, ValueType v1, ValueType v2) {
  switch (v1.type) {
    case INTEGER:
      return Arithmetic<int64_t>(op, get<int64_t>(v1.Value), v2);
    case UINTEGER:
      return Arithmetic<uint64_t>(op, get<uint64_t>(v1.Value), v2);
    case FLOAT:
      return Arithmetic<double>(op, get<double>(v1.Value), v2);
    default:
      return LiftError<VType>(INVALID_ARGUMENT, "");
  }
}

}  // namespace dlvm
