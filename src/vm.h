#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "opcodes.h"
#include "types.h"

typedef struct {
    char *name;
    uint64_t start;
    uint64_t end;
} heap_obj_t;

typedef struct {
    uint64_t heap_size;
    uint64_t *heap;
    heap_obj_t *heap_objs;
} heap_t

typedef struct {
    uint64_t stack_size;
    uint64_t *stack;
} heap_t

typedef struct {
    uint64_t program_size;
    uint64_t *program;
} program_t;

typedef struct {
    program_t *program;
    stack_t *stack;
    heap_t *heap;

    uint64_t pc = 0;
    uint64_t sp = 0;
    uint64_t fp = 0;
} dlvm_t;

dlvm_t *dlvm_init(program_t *p, stack_t *s, heap_t *h);

void dlvm_exec(dlvm_t *vm);

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