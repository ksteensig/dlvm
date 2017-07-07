#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "types.h"

/*
typedef struct {
    char *name;
    uint64_t start;
    uint64_t end;
} heap_obj_t;
*/

/*
typedef struct {
    uint64_t heap_size;
    uint64_t *heap;

    uint64_t heap_objs_count;
    heap_obj_t *heap_objs;
} heap_t;
*/

typedef struct {
    uint64_t stack_size;
    ttype_t **stack;
} stack_t;

typedef struct {
    uint64_t program_size;
    uint64_t *program;
} program_t;

typedef struct {
    program_t *program;
    stack_t *stack;
    //heap_t *heap;

    uint64_t pc;
    uint64_t sp;
    uint64_t fp;
} dlvm_t;

dlvm_t *dlvm_init(program_t *p, stack_t *s);

program_t *program_init(char *filename);

stack_t *stack_init(uint64_t stack_size);

//heap_t *heap_init(uint64_t heap_size);

//heap_t *heap_obj_init(char *name, uint64_t start, uint64_t end);

bool check_and_print_error(dlvm_t *vm);