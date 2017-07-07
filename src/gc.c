#include "gc.h"

void dlvm_gc_mark(dlvm_t *vm) {
    for (uint64_t i = 0; i < vm->stack->stack_size; i++) {
        if (vm->stack->stack[i] != NULL) {
            vm->stack->stack[i]->marked = true;
        } else {
            break;
        }
    }
}

void dlvm_gc_sweep(dlvm_t *vm) {
    ttype_t *current = vm->stack->stack[0];
    ttype_t *next;

    while (current != NULL) {
        if (current->marked == false) {
            next = current->next;
            switch (current->t) {
                case BOOL:
                    free_bool(current);
                    break;
                case INT:
                    free_int(current);
                    break;
                case FLOAT:
                    free_float(current);
                    break;
                case CHAR:
                    free_char(current);
                    break;
                case LIST:
                    free_list(current);
                    break;
                case FUNCTION:
                    free_fun(current);
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
            vm->stack->stack[i]->marked = false;
            vm->stack->stack[i]->next = vm->stack->stack[i + 1];
        } else {
            break;
        }
    }
}