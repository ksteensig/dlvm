#pragma once

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

namespace dlvm {

using namespace dlvm;
using namespace std;

struct Type {
    bool Marked = false;
    unique_ptr<Type> Next;
};

template<class T>
struct TNumber : Type {
    T Value;

    TNumber(T V)
        : Value{V}
    { }
};

struct TBool : Type {
    bool Bool;

    TBool(bool B)
        : Bool{B}
    { }
};

struct TString : Type {
    string String;

    TString(string S)
        : String{S}
    { }
};

struct TList : Type {
    vector<unique_ptr<Type>> List;

    void Append(unique_ptr<Type> obj);
    void Prepend(unique_ptr<Type> obj);
    void Set(unique_ptr<Type>, uint64_t pos);
    void Insert(unique_ptr<Type> obj, uint64_t pos);
    unique_ptr<Type> Access(uint64_t pos);
};

}