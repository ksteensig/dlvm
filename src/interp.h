#pragma once

#include "vm.h"
#include "opcodes.h"
#include "gc.h"

bool check_and_print_error(dlvm_t *vm);

void dlvm_exec(dlvm_t *vm);

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

void _INSER_LIST(dlvm_t *vm);
void _SET_LIST(dlvm_t *vm);
void _ACCESS_LIST(dlvm_t *vm);

void _INVOKE(dlvm_t *vm);
void _CALL(dlvm_t *vm);
void _RET(dlvm_t *vm);

void _JMP(dlvm_t *vm);
void _JMPF(dlvm_t *vm);
void _JMPT(dlvm_t *vm);