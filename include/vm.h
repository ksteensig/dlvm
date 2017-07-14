#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "types.h"
#include "opcodes.h"

typedef struct {
	uint8_t *program;
	uint64_t program_size;

	ttype_t **stack;
	uint64_t stack_size;

    uint64_t pc;
    uint64_t sp;
    uint64_t fp;
} dlvm_t;

dlvm_t *dlvm_init(program_t *p, stack_t *s);

program_t *program_init(char *filename);

stack_t *stack_init(uint64_t stack_size);

void dlvm_push(dlvm_t *vm, ttype_t *v);
ttype_t *dlvm_pop(dlvm_t *vm);
uint64_t dlvm_next_op(dlvm_t *vm);
