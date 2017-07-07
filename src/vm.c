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

void dlvm_exec(dlvm_t *vm) {
    opcode_t opcode;
    ttype_t *r1, *r2, *res;
    uint8_t gc = 0;

    while (1) {
        opcode = dlvm_next_op(vm);
        switch(opcode) {
            case ADD:
                r1 = dlvm_pop(vm);
                r2 = dlvm_pop(vm);
                res = add(r1, r2);
                dlvm_push(vm, res);
                break;
            case MUL:
                r1 = dlvm_pop(vm);
                r2 = dlvm_pop(vm);
                res = multiply(r1, r2);
                dlvm_push(vm, res);
                break;
            case SUB:
                r1 = dlvm_pop(vm);
                r2 = dlvm_pop(vm);
                res = subtract(r1, r2);
                dlvm_push(vm, res);
                break;
            case DIV:
                r1 = dlvm_pop(vm);
                r2 = dlvm_pop(vm);
                res = divide(r1, r2);
                dlvm_push(vm, res);
                break;
            case MOD:
                r1 = dlvm_pop(vm);
                r2 = dlvm_pop(vm);
                res = modulo(r1, r2);
                dlvm_push(vm, res);
                break;
            case AND:
                r1 = dlvm_pop(vm);
                r2 = dlvm_pop(vm);
                res = and(r1, r2);
                dlvm_push(vm, res);
            case OR:
                r1 = dlvm_pop(vm);
                r2 = dlvm_pop(vm);
                res = or(r1, r2);
                dlvm_push(vm, res);
            case NOT:
                r1 = dlvm_pop(vm);
                res = not(r1);
                dlvm_push(vm, res);
            case PUSH:
                switch (dlvm_next_op(vm)) {
                    case BOOL:
                        r1 = init_int((bool)dlvm_next_op(vm));
                    case INT:
                        r1 = init_int(dlvm_next_op(vm));
                        break;
                    case FLOAT:
                        r1 = init_float((double)dlvm_next_op(vm));
                        break;
                    case CHAR:
                        r1 = init_char((char)dlvm_next_op(vm));
                    case LIST:
                        r1 = init_list();
                    case FUNCTION:
                        // first arg is argc and second arg is addr
                        r1 = init_fun(dlvm_next_op(vm), dlvm_next_op(vm));
                    default:
                        return;
                }
                dlvm_push(vm, r1);
                break;
            case LOAD:
                r1 = dlvm_pop(vm);
                dlvm_push(vm, vm->stack->stack[vm->fp - (uint64_t)((tint_t *)r1)->v]);
                break;
            case GLOAD:
                r1 = dlvm_pop(vm);
                dlvm_push(vm, vm->stack->stack[(uint64_t)((tint_t *)r1)->v]);
                break;
            case POP:
                dlvm_pop(vm);
                break;
            case INSERT_LIST:
                r1 = dlvm_pop(vm);
                r2 = dlvm_pop(vm);
                res = insert_into_list(r1, r2, dlvm_next_op(vm));
                dlvm_push(vm, res);
                break;
            case SET_LIST:
                r1 = dlvm_pop(vm);
                r2 = dlvm_pop(vm);
                res = overwrite_list(r1, r2, dlvm_next_op(vm));
                dlvm_push(vm, res);
                break;
            case ACCESS_LIST:
                r1 = dlvm_pop(vm);
                res = access_list(r1, dlvm_next_op(vm));
                dlvm_push(vm, res);
                break;
            case INVOKE: // invoke function object
                r1 = dlvm_pop(vm);
                dlvm_push(vm, init_int(((tfun_t *)r1)->argc));
                dlvm_push(vm, init_int(vm->fp));
                dlvm_push(vm, init_int(vm->pc));
                vm->fp = vm->sp;
                vm->pc = ((tfun_t *)r1)->addr;
                break;
            case CALL: // invoke procedure
                dlvm_push(vm, init_int(dlvm_next_op(vm))); // push argc to stack
                dlvm_push(vm, init_int(vm->fp));
                dlvm_push(vm, init_int(vm->pc));
                vm->fp = vm->sp;
                vm->pc = dlvm_next_op(vm); // addr
                break;
            case RET:
                r1 = dlvm_pop(vm); // get result
                vm->sp = vm->fp;
                vm->pc = (uint64_t)((tint_t *)dlvm_pop(vm))->v;
                vm->fp = (uint64_t)((tint_t *)dlvm_pop(vm))->v;
                vm->sp -= (uint64_t)((tint_t *)dlvm_pop(vm))->v;
                dlvm_push(vm, r1); // push result back to stack
                break;
            case JMP:
                vm->pc = (uint64_t)dlvm_next_op(vm);
                break;
            case PRINT:
                r1 = dlvm_pop(vm);
                printf("%ld\n", ((tint_t *)r1)->v);
            case HALT:
                dlvm_gc_mark_and_sweep(vm);
                return;
            default:
                return;
        }

        if (check_and_print_error(vm)) {
            return;
        }

        if (!(++gc)) {
            dlvm_gc_mark_and_sweep(vm);
        }
    }
}