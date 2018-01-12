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
    vector<shared_ptr<Type>> Stack; 
    uint64_t SP = 0;
    uint64_t PC = 0;
    uint64_t FP = 0;

    VM(unique_ptr<vector<uint8_t>> P)
        : Program{move(P)}
    { }

    shared_ptr<Type> Pop();
    void Push(shared_ptr<Type> obj);
    uint8_t Next();
    void Execute();
    void GarbageCollect();
    void POP_HANDLER();
    void PUSH_INT_HANDLER();
    void PRINT_HANDLER();
    void ADD_HANDLER();
};

}