#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <optional>
#include <algorithm>

#include "opcode.hpp"
#include "type.hpp"
#include "vmm.hpp"

namespace dlvm {

using namespace std;
using namespace dlvm;

class Interpreter {
    MemoryManager Memory;

    unique_ptr<uint8_t[]> Program;
    uint64_t pc = 0;

    ValueType reg1, reg2, reg3, reg4;

    Result<ValueType> Pop();
    Result<ValueType> Push(ValueType obj);

    uint8_t Next();
    uint64_t NextEightBytes();

    Result<ValueType> POP_HANDLER();
    Result<ValueType> PUSH_INT_HANDLER();
    Result<ValueType> ADD_HANDLER();
    Result<ValueType> CREATE_REFERENCE_HANDLER();
    Result<ValueType> PRINT_HANDLER();

    public:
    Interpreter(unique_ptr<uint8_t[]> program, map<string, uint32_t> settings)
        : Program{move(program)}
        , Memory{MemoryManager{settings}}
    { }

    void Execute();
};

}