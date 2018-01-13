#pragma once

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <optional>

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

struct Type {
    type_t type;
    bool Marked = false;
    optional<Type> Next;

    virtual void Print() = 0;
};

struct TError : Type {
    void Print() { }
};

struct TInteger : Type {
    int64_t Value;

    TInteger (int64_t V)
        : Value{V}
    { type = INTEGER; }

    void Print() {cout << Value;}
};

struct TFloat : Type {
    double Value;

    TFloat (double V)
        : Value{V}
    { type = FLOAT; }

    void Print() {cout << Value;}
};

struct TBool : Type {
    bool Bool;

    TBool(bool B)
        : Bool{B}
    { type = BOOL; }

    void Print() {cout << Bool;}
};

struct TString : Type {
    string String;

    TString(string S)
        : String{S}
    { type = STRING; }

    void Print() {cout << String;}
};

struct TReference : Type {
    uint32_t Address;

    TReference(uint32_t addr)
        : Address{addr}
    { type = REFERENCE; }
};


struct TArray : TReference {
    uint32_t Length;

    void Insert(shared_ptr<Type> obj, uint32_t pos);
    optional<Type> Access(uint32_t pos);

    TArray(uint32_t len)
        : Length(len)
    { type = ARRAY; }

    void Print() {}
};

}