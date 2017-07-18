#pragma once

#include "vm.h"
#include "opcodes.h"
#include "gc.h"
#include "utf8.h"
#include "types.h"

void dlvm_parse_header(dlvm_module_t *module);
void dlvm_exec(dlvm_t *vm);

void dlvm_push(dlvm_t *vm, ttype_t *o);
ttype_t *dlvm_pop(dlvm_t *vm);
uint8_t dlvm_next_op(dlvm_t *vm);

bool check_and_print_error(dlvm_t *vm);

void _ADD(dlvm_t *vm);
void _SUB(dlvm_t *vm);
void _MUL(dlvm_t *vm);
void _DIV(dlvm_t *vm);
void _MOD(dlvm_t *vm);
void _AND(dlvm_t *vm);
void _OR(dlvm_t *vm);
void _NOT(dlvm_t *vm);

void _PUSH(dlvm_t *vm);
void _POP(dlvm_t *vm);
void _LOAD(dlvm_t *vm);
void _GLOAD(dlvm_t *vm);

void _INSERT_LIST(dlvm_t *vm);
void _SET_LIST(dlvm_t *vm);
void _ACCESS_LIST(dlvm_t *vm);

void _INVOKE(dlvm_t *vm);
void _CALL(dlvm_t *vm);
void _RET(dlvm_t *vm);

void _JMP(dlvm_t *vm);
void _JMPF(dlvm_t *vm);
void _JMPT(dlvm_t *vm);

void _CMP_EQ(dlvm_t *vm);
void _CMP_L(dlvm_t *vm);
void _CMP_G(dlvm_t *vm);
void _CMP_LE(dlvm_t *vm);
void _CMP_GE(dlvm_t *vm);

void _PRINT(dlvm_t *vm);
