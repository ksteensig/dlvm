#pragma once

#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "opcodes.h"
#include "types.h"

using namespace std;

#define PUSH(vm, v) vm->stack[++vm->sp] = v
#define POP(vm)     vm->stack[vm->sp--]
#define NCODE(vm)   vm->program[vm->pc++] // get next opcode

class VM {
public:
    vector<uint32_t> Program;
    vector<uint32_t> Stack;
    vector<uint32_t> Heap;
    uint32_t pc = 0;
    uint32_t sp = 0;
    uint32_t fp = 0;

    void Exec();
    void Push(uint32_t v);
    uint32_t Pop();
    uint32_t Next();
};