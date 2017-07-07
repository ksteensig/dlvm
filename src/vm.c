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

void dlvm_gc_mark_and_sweep(dlvm_t *vm) {
    dlvm_gc_mark(vm);
    printf("marked\n");
    dlvm_gc_sweep(vm);
    printf("sweeped\n");
    dlvm_gc_reset_marked(vm);
    printf("reset\n");
}

bool check_and_print_error(dlvm_t *vm) {
    ttype_t *typ = vm->stack->stack[vm->sp - 1];
    terror_t *err;

    if (typ->t == ERROR) {
        err = (terror_t *)typ;
        printf(err->msg);
        return true;
    }

    return false;
}