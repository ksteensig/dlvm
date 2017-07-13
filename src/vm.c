#include "vm.h"

dlvm_t *dlvm_init(uint8_t *p, uint64_t ps, uint64_t ss) {
    dlvm_t *vm = malloc(sizeof(dlvm_t));

    vm->program = p;
    vm->program_size = ps;

	/* initializes the stack to the given stack_size */ 
	vm->stack = malloc(sizeof(ttype_t *) * ss);
	vm->stack_size = ss;

    vm->pc = 0;
    vm->sp = 0;
    vm->fp = 0;

    return vm;
}

uint64_t dlvm_next_op(dlvm_t *vm) {
    return vm->program->program[vm->pc++];
}

void dlvm_push(dlvm_t *vm, ttype_t *o) {
	/* keep track of what last was pushed,
	 * this is used by the garbage collector
	 * to build a linked list of objects
	 */
    static ttype_t *last_pushed;

    if (last_pushed != NULL) {
         o->next = last_pushed;
    }
    
    last_pushed = o;

    vm->stack[vm->sp++] = o;
}

ttype_t *dlvm_pop(dlvm_t *vm) {
	/* pre-decrements the stack pointer, before returning the object */
    ttype_t *o = vm->stack->stack[--(vm->sp)];

	/* sets the position where the object was on the stack to NULL */
    vm->stack->stack[vm->sp + 1] = NULL;

    return o;
}
