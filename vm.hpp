#pragma once

#include <vector>
#include <stdint.h>
#include <memory>
#include <optional>

#include "opcode.hpp"
#include "type.hpp"

namespace dlvm {

using namespace std;
using namespace dlvm;

class VM {
    unique_ptr<vector<uint8_t>> Program;
    unique_ptr<vector<Type>> Stack;
    unique_ptr<Type[]> Heap;
    unique_ptr<vector<optional<addr_t>>> PageTable;
    uint64_t PC = 0;
    addr_t SP = 0;
    addr_t FP = 0;
    addr_t HP = 0;
    uint32_t ssize = 0;
    uint32_t hsize = 0;

    Type r1, r2, r3, r4;

    Type Pop();
    void Push(Type obj);

    uint64_t GetQuad();

    void GarbageCollect();
    Result<addr_t> Malloc(uint32_t size);
    Result<addr_t> TranslateAddress(addr_t addr);

    public:
    VM(unique_ptr<vector<uint8_t>> P, uint32_t ssize, uint32_t hsize)
        : Program{move(P)}
        , ssize{ssize}
        , hsize{hsize}
    {
        Heap = make_unique<Type[]>(hsize);
    }

    uint8_t Next();
    void Execute();
    Result<Type> POP_HANDLER();
    Result<Type> PUSH_INT_HANDLER();
    Result<Type> ADD_HANDLER();
    Result<Type> CREATE_REFERENCE_HANDLER();
    void PRINT_HANDLER();
    
};

}