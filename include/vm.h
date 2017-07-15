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

	tfun_t **function_table;
	function_table_size;

	texception_t *exception_table;
	uint64_t *exception_table_size;

	texception_handler_t *handler;

    uint64_t pc;
    uint64_t sp;
    uint64_t fp;
} dlvm_t;

dlvm_t *dlvm_init(uint8_t *p, uint64_t ps, uint64_t ss);

void dlvm_push(dlvm_t *vm, ttype_t *o);
ttype_t *dlvm_pop(dlvm_t *vm);
uint8_t dlvm_next_op(dlvm_t *vm);
