#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "types.h"
#include "opcodes.h"

typedef struct {
	/* program bytecode */
    uint8_t *program;					// program bytecode
	uint64_t program_size;				// size of program

	/* stack */
    ttype_t **stack;					// array of pointers to ttype_t
	uint64_t stack_size;				// max size of the stack

	tfun_t **function_table;			// table with program functions
	uint64_t function_table_size;		// size of function table

	ex_handler_t *exception_handler;	// last ex handler in a linked list

	/* registers */
    uint64_t pc;						// program counter
    uint64_t sp;						// stack pointer
    uint64_t fp;						// frame pointer
} dlvm_t;

/* p is program, ps is program_size, ss is stack_size */
dlvm_t *dlvm_init(uint8_t *p, uint64_t ps, uint64_t ss);

/* vm is the virtual machine, o is the object pushed on the stack */
void dlvm_push(dlvm_t *vm, ttype_t *o);

/* vm is the virtual machine which has an object popped from its stack */
ttype_t *dlvm_pop(dlvm_t *vm);

/* gets next opcode from the program */
uint8_t dlvm_next_op(dlvm_t *vm);
