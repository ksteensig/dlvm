#ifndef LISP_LANG_VM_H
#define LISP_LANG_VM_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "opcodes.h"
#include "types.h"

#define PUSH(vm, v) vm->stack[++vm->sp] = v
#define POP(vm)     vm->stack[vm->sp--]
#define NCODE(vm)   vm->program[vm->pc++] // get next opcode

typedef struct vm_e {
    uint32_t *program;
    uint32_t *stack;
    uint32_t *heap;
    uint32_t pc;
    uint32_t sp;
    uint32_t fp;
} vm_t;

vm_t *vm_init(uint32_t *prog, uint32_t stack_size, uint32_t heap_size) {
    vm_t *vm = malloc(sizeof(vm_t));
    vm->stack = malloc(sizeof(uint32_t) * stack_size);
    vm->heap = malloc(sizeof(uint32_t) * heap_size);
    vm->program = prog;
    vm->pc = 0;
    vm->sp = 0;
    vm->fp = 0;

    return vm;
}

void vm_del(vm_t *vm) {
    free(vm->stack);
    free(vm->heap);
    free(vm->program);
    free(vm);
}

int exec(vm_t *vm);

#endif