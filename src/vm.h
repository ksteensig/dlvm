#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "opcodes.h"
#include "types.h"

/*
class VM {
public:
    vector<uint32_t> Program;
    vector<intptr_t> Stack;
    uint32_t pc = 0;
    uint32_t sp = 0;
    uint32_t fp = 0;

    void Exec();
    void Push(intptr_t v);
    intptr_t Pop();
    uint32_t Next();
};
*/