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

template <typename Type>
struct Result {
  result_t rtype;
  variant<Error, Type> result;

  Result(result_t rtype, variant<Error, Type> result)
      : rtype{rtype}, result{result} {}

  bool isError() { return rtype == ERROR; }
  bool isType() { return rtype == OK; }
};

template <typename RType, typename PType>
Result<RType> MapResult(function<Result<RType>(PType)> f, Result<PType> r);

/*
template <typename RType, typename PType, typename OType>
Result<RType> MapResult2(function<Result<RType>(PType, OType)> f,
                         Result<PType> r1, Result<OType> r2);
*/

template <typename Type>
Result<Type> LiftError(ErrorCode error_code, string msg) {
  return Result<Type>{ERROR, Error{error_code, msg}};
}

template <typename Type>
Result<Type> LiftType(Type t) {
  return Result<Type>{OK, t};
}

template <typename RType, typename PType>
Result<RType> MapResult(function<Result<RType>(PType, PType)> f,
                        Result<PType> r) {
  switch (r.rtype) {
    case ERROR:
      return LiftError<RType>(INVALID_ARGUMENT, "");
    case OK:
      return f(r);
  }
}

template <typename RType, typename PType, typename OType>
Result<RType> MapResult2(function<Result<RType>(PType, OType)> f,
                         Result<PType> r1, Result<OType> r2) {
  switch (r1.rtype) {
    case ERROR:
      return r1;
    case OK:
      switch (r2.rtype) {
        case ERROR:
          return r2;
        case OK:
          return f(get<PType>(r1.result), get<OType>(r2.result));
        default:
          return LiftError<RType>(INVALID_ARGUMENT, "");
      }
      break;
    default:
      return LiftError<RType>(INVALID_ARGUMENT, "");
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

struct Arithmetic2 {
  ArithmeticOperator op;
  Arithmetic2(ArithmeticOperator op) : op{op} {}
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

}  // namespace dlvm
