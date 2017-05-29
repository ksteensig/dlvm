#include "vm.h"

dlvm_t *dlvm_init(program_t *p, stack_t *s) {
    dlvm_t *vm = malloc(sizeof(dlvm_t));

    vm->program = p;
    vm->stack = s;

    vm->pc = 0;
    vm->sp = 0;
    vm->fp = 0;

    return vm;
}

program_t *program_init(char *filename) {
    program_t *prog = malloc(sizeof(program_t));

    return prog;
}

stack_t *stack_init(uint64_t stack_size) {
    stack_t *stack = malloc(sizeof(stack_t));

    stack->stack_size = stack_size;
    stack->stack = malloc(sizeof(uint64_t) * (stack_size + 1));

    return stack;
}

/*
heap_t *heap_init(uint64_t heap_size) {
    heap_t *heap = malloc(sizeof(heap_t));

    heap->heap_size = heap_size;
    heap->heap = malloc(sizeof(uint64_t) * heap_size);

    heap->heap_objs_count = 0;

    return heap;
}
*/

uint64_t dlvm_next_op(dlvm_t *vm) {
    return vm->program->program[++(vm->pc)];
}

void dlvm_push(dlvm_t *vm, ttype_t *v) {
    vm->stack->stack[++vm->sp] = v;
}

ttype_t *dlvm_pop(dlvm_t *vm) {
    return vm->stack->stack[vm->sp--];
}

void dlvm_gc_mark(dlvm_t *vm) {
    for (uint64_t i = 0; i < vm->stack->stack_size; i++) {
        if (vm->stack->stack[i] != NULL) {
            ((ttype_t *)vm->stack->stack[i])->marked = 1;
        } else {
            break;
        }
    }
}

void dlvm_gc_sweep(dlvm_t *vm) {
    ttype_t *current = vm->stack->stack[0];
    ttype_t *next;

    while (current != NULL) {
        if (current->marked == 0) {
            next = current->next;
            free(current);
            current = next;
        }
    }

    for (uint64_t i = 0; i < vm->stack->stack_size; i++) {
        if (vm->stack->stack[i] != NULL) {
            ((ttype_t *)vm->stack->stack[i])->marked = 0;
            ((ttype_t *)vm->stack->stack[i])->next = vm->stack->stack[i + 1];
        } else {
            break;
        }
    }
}

void dlvm_exec(dlvm_t *vm) {
    opcode_t opcode;
    ttype_t *a, *b;

    while (1) {
        opcode = dlvm_next_op(vm);

        switch(opcode) {
            case ADD:
                
            case PUSH:
                a = malloc(sizeof(tint_t));
                ((tint_t *)a)->v = dlvm_next_op(vm);
                ((tint_t *)a)->t = INT;
                dlvm_push(vm, a);
                printf("pushed val\n");
                break;
            case POP: break;
            case HALT: return;;
            default: return;
        }
    }
}