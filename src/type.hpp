#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <variant>

namespace dlvm {

using namespace dlvm;
using namespace std;

typedef uint64_t vaddr_t;
typedef uint32_t addr_t;

typedef uint32_t array_t;
typedef variant<int64_t, uint64_t, double, bool, char, addr_t> VType;
typedef variant<array_t, VType> RType;

struct ValueType;
struct ReferenceType;
template<typename Type>
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
    STRUCT
} type_t;

struct ValueType {
    type_t type;
    VType Value;

    ValueType()
    { type = NIL; }

    ValueType(type_t type, VType value)
        : type{type}
        , Value{value}
    { }

    ReferenceType Box();
};

struct ReferenceType {
    type_t type;
    bool Marked = false;
    RType Value;

    ReferenceType()
    { type = NIL; }

    ReferenceType(ValueType value)
        : type{value.type}
        , Value{value.Value}
    { }

    ReferenceType(type_t type, RType value)
        : type{type}
        , Value{value}
    { }

    Result<ValueType> Unbox();
};

typedef enum ErrorCode {
    OK,
    OUT_OF_MEMORY,
    OUT_OF_BOUNDS,
    STACK_OVERFLOW,
    TYPE_ERROR,
    NULL_REFERENCE,
    INVALID_ARGUMENT,
    UNKNOWN
} ErrorCode;

template<typename Type>
struct Result {
    ErrorCode ErrCode;
    Type Value;
    string Message;

    Result(Type value, string msg, ErrorCode error_code)
        : Value{value}
        , Message{msg}
        , ErrCode{error_code}
    { }

    Result(Type value)
        : Value{value}
        , ErrCode{OK}
    { }

    Result(string msg, ErrorCode error_code)
        : Message{msg}
        , ErrCode{error_code}
    { }
};

template<typename Type>
Result<Type> OkResult(Type value);
template<typename Type>
Result<Type> TypeError(Type value);
template<typename Type>
Result<Type> ThrowError(string msg, Type value, ErrorCode error_code);
template<typename Type>
Result<Type> ThrowError(string msg, ErrorCode error_code);

/*
Result operator+ (Result lhs, Result rhs);
Result operator- (Type lhs, Type rhs);
Result operator* (Type lhs, Type rhs);
Result operator/ (Type lhs, Type rhs);
Result operator&& (Type lhs, Type rhs);
Result operator|| (Type lhs, Type rhs);
Result operator! (Type rhs);
ostream& operator<< (ostream &o, Type t);
*/
}