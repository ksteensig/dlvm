#include "interp.h"

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