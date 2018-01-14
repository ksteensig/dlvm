#pragma once

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include <variant>

namespace dlvm {

using namespace dlvm;
using namespace std;

typedef enum type_e {
    NIL,
    INTEGER,
    FLOAT,
    BOOL,
    STRING,
    ARRAY,
    REFERENCE
} type_t;

typedef uint32_t addr_t;
typedef tuple<addr_t, uint32_t> array_t;
typedef variant<int64_t, double, bool, char, addr_t, array_t> TypeCollection;

struct Type {
    type_t type;
    bool Marked = false;
    optional<addr_t> Next;

    TypeCollection Value;

    Type()
    { type = NIL; }

    Type(type_t t, TypeCollection val)
        : type{t}
        , Value{val}
    { }
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

Result OkResult(Type value);
Result TypeError(Type value);
Result ThrowError(string msg, Type value, ErrorCode error_code);
Result ThrowError(string msg, ErrorCode error_code);

Result operator+ (Result lhs, Result rhs);
Result operator- (Type lhs, Type rhs);
Result operator* (Type lhs, Type rhs);
Result operator/ (Type lhs, Type rhs);
Result operator&& (Type lhs, Type rhs);
Result operator|| (Type lhs, Type rhs);
Result operator! (Type rhs);
ostream& operator<< (ostream &o, Type t);

}