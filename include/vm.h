#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "types.h"
#include "utf8.h"

typedef struct dlvm_module_s {
	utf8_t *path;
	utf8_t *name;
	tfun_t **function_table;
	uint64_t table_size;
	uint64_t program_start;
} dlvm_module_t;

typedef struct dlvm_s {
	uint8_t *program;
	uint64_t program_size;

	ttype_t **stack;
	uint64_t stack_size;

	dlvm_module_t *module_table;
	uint64_t module_table_size;

	texception_handler_t *handler;

    uint64_t pc;
    uint64_t sp;
    uint64_t fp;
} dlvm_t;

dlvm_module_t *dlvm_module_init(uint8_t *path);
void dlvm_module_free(dlvm_module_t *module);

dlvm_t *dlvm_init(uint8_t *p, uint64_t ps, uint64_t ss);
void dlvm_free(dlvm_t *vm);
