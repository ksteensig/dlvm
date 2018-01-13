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
    UNKNOWN
} ErrorCode;

template<class T>
struct Result {
    public:
    ErrorCode ErrCode;
    T Value;
    string Message;

    Result(T value, string msg, ErrorCode error_code)
        : Value{value}
        , Message{msg}
        , ErrCode{error_code}
    { }
};

template<class T>
Result<T> OkResult(type_t type, TypeCollection value);

template<class T>
Result<T> TypeError(type_t type, TypeCollection value);

Result<Type> operator+ (Type lhs, Type rhs);
Result<Type> operator- (Type lhs, Type rhs);
Result<Type> operator* (Type lhs, Type rhs);
Result<Type> operator/ (Type lhs, Type rhs);
Result<Type> operator&& (Type lhs, Type rhs);
Result<Type> operator|| (Type lhs, Type rhs);
Result<Type> operator! (Type rhs);
ostream& operator<< (ostream &o, Type t);

}