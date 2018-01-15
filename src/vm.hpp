#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <optional>
#include <algorithm>

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

    Result Pop();
    Result Push(Type obj);

    uint64_t GetEightBytes();

    optional<addr_t> last_in_heap;
    uint32_t Mark(addr_t addr);
    void Compact(uint32_t marked);
    void Move(addr_t heap_addr);
    void GarbageCollect();
    Result Malloc(uint32_t size);
    Result TranslateAddress(addr_t addr);

    public:
    VM(unique_ptr<vector<uint8_t>> P, uint32_t ssize, uint32_t hsize)
        : Program{move(P)}
        , ssize{ssize}
        , hsize{hsize}
    {
        Stack = make_unique<vector<Type>>();
        Heap = make_unique<Type[]>(hsize);
        PageTable = make_unique<vector<optional<addr_t>>>(hsize, optional<addr_t>{});
        optional<addr_t> last_in_heap{};
    }

    uint8_t Next();
    void Execute();
    Result POP_HANDLER();
    Result PUSH_INT_HANDLER();
    Result ADD_HANDLER();
    Result CREATE_REFERENCE_HANDLER();
    Result PRINT_HANDLER();
};

}