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
    stack->stack = malloc(sizeof(ttype_t*) * stack_size);

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
    return vm->program->program[vm->pc++];
}

void dlvm_push(dlvm_t *vm, ttype_t *v) {
    static ttype_t *last_pushed;

    if (last_pushed != NULL) {
         v->next = last_pushed;
    }
    
    last_pushed = v;

    vm->stack->stack[vm->sp++] = v;
}

ttype_t *dlvm_pop(dlvm_t *vm) {
    ttype_t *ret = vm->stack->stack[--(vm->sp)];
    vm->stack->stack[vm->sp + 1] = NULL;

    return ret;
}

void dlvm_gc_mark(dlvm_t *vm) {
    for (uint64_t i = 0; i < vm->stack->stack_size; i++) {
        if (vm->stack->stack[i] != NULL) {
            vm->stack->stack[i]->marked = 1;
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
            
            switch (current->t) {
                case INT:
                    free_int(current);
                    break;
                default:
                    free(current);
            }

            current = next;
        } else {
            next = current->next;
            current = next;
        }
    }
}

void dlvm_gc_reset_marked(dlvm_t *vm) {
    for (uint64_t i = 0; i < vm->stack->stack_size; i++) {
        if (vm->stack->stack[i] != NULL) {
            vm->stack->stack[i]->marked = 0;
            vm->stack->stack[i]->next = vm->stack->stack[i + 1];
        } else {
            break;
        }
    }
}

void dlvm_gc_mark_and_sweep(dlvm_t *vm) {
    dlvm_gc_mark(vm);
    dlvm_gc_sweep(vm);
    dlvm_gc_reset_marked(vm);
}

void dlvm_exec(dlvm_t *vm) {
    opcode_t opcode;
    ttype_t *r1, *r2, *res;

    uint8_t gc = 1;

    while (1) {
        opcode = dlvm_next_op(vm);
        switch(opcode) {
            case ADD:
                r1 = dlvm_pop(vm);
                r2 = dlvm_pop(vm);
                res = add_int(r1, r2);
                dlvm_push(vm, res);
                break;
            case PUSH:
                r1 = init_int(dlvm_next_op(vm));
                dlvm_push(vm, r1);
                break;
            case POP:
                dlvm_pop(vm);
                break;
            case HALT:
                dlvm_gc_mark_and_sweep(vm);
                return;
            default:
                return;
        }

        if (!gc++) {
            dlvm_gc_mark_and_sweep(vm);
        }
    }
}