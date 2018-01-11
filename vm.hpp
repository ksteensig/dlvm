#pragma once

#include <vector>
#include <stdint.h>
#include <memory>

#include "opcode.hpp"
#include "type.hpp"

namespace dlvm {

using namespace std;
using namespace dlvm;

class VM {
    public:
    unique_ptr<vector<uint8_t>> Program;
    unique_ptr<vector<unique_ptr<Type>>> Stack; 
    uint64_t SP = 0;
    uint64_t PC = 0;
    uint64_t FP = 0;

    VM(unique_ptr<vector<uint8_t>> P,
        unique_ptr<vector<unique_ptr<Type>>> S)
        : Program{move(P)}
        , Stack{move(S)}
    { }

    unique_ptr<Type> Pop();
    void Push(unique_ptr<Type> obj);
    uint8_t Next();
    void Interpret();
    void GarbageCollect();
};

}