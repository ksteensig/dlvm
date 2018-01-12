#pragma once

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

namespace dlvm {

using namespace dlvm;
using namespace std;

typedef enum type_e {
    NIL,
    INTEGER,
    FLOAT,
    BOOL,
    STRING,
    LIST
} type_t;

struct Type {
    type_t type;
    bool Marked = false;
    shared_ptr<Type> Next;
    virtual void Print() = 0;
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

struct TList : Type {
    vector<shared_ptr<Type>> List;

    void Append(shared_ptr<Type> obj);
    void Prepend(shared_ptr<Type> obj);
    void Set(shared_ptr<Type>, uint64_t pos);
    void Insert(shared_ptr<Type> obj, uint64_t pos);
    shared_ptr<Type> Access(uint64_t pos);

    TList ()
    { type = LIST; }

    void Print() {}
};

}