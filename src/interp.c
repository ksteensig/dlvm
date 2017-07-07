#include "interp.h"

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

void _ADD(dlvm_t *vm) {
    ttype_t *r1, *r2, *res;
    r1 = dlvm_pop(vm);
    r2 = dlvm_pop(vm);
    res = add(r1, r2);
    dlvm_push(vm, res);
}

void _SUB(dlvm_t *vm) {
    ttype_t *r1, *r2, *res;
    r1 = dlvm_pop(vm);
    r2 = dlvm_pop(vm);
    res = subtract(r1, r2);
    dlvm_push(vm, res);
}

void _MUL(dlvm_t *vm) {
    ttype_t *r1, *r2, *res;
    r1 = dlvm_pop(vm);
    r2 = dlvm_pop(vm);
    res = multiply(r1, r2);
    dlvm_push(vm, res);
}

void _DIV(dlvm_t *vm) {
    ttype_t *r1, *r2, *res;
    r1 = dlvm_pop(vm);
    r2 = dlvm_pop(vm);
    res = divide(r1, r2);
    dlvm_push(vm, res);
}

void _MOD(dlvm_t *vm) {
    ttype_t *r1, *r2, *res;
    r1 = dlvm_pop(vm);
    r2 = dlvm_pop(vm);
    res = modulo(r1, r2);
    dlvm_push(vm, res);
}

void _AND(dlvm_t *vm) {
    ttype_t *r1, *r2, *res;
    r1 = dlvm_pop(vm);
    r2 = dlvm_pop(vm);
    res = and(r1, r2);
    dlvm_push(vm, res);
}

void _OR(dlvm_t *vm) {
    ttype_t *r1, *r2, *res;
    r1 = dlvm_pop(vm);
    r2 = dlvm_pop(vm);
    res = or(r1, r2);
    dlvm_push(vm, res);
}

void _NOT(dlvm_t *vm) {
    ttype_t *r1, *res;
    r1 = dlvm_pop(vm);
    res = not(r1);
    dlvm_push(vm, res);
}

void _PUSH(dlvm_t *vm) {
    ttype_t *r1;
    uint64_t type = dlvm_next_op(vm);

    switch (type) {
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
            break;
        case LIST:
            r1 = init_list();
            break;
        case FUNCTION:
            // first arg is argc and second arg is addr
            r1 = init_fun(dlvm_next_op(vm), dlvm_next_op(vm));
        default:
            return;
    }
    
    dlvm_push(vm, r1);
}

void _POP(dlvm_t *vm) {
    dlvm_pop(vm);
}

void _LOAD(dlvm_t *vm) {
    tint_t *offset;
    ttype_t *r1 = dlvm_pop(vm);

    if (r1->t == INT) {
        offset = (tint_t *)r1;
        dlvm_push(vm, vm->stack->stack[vm->fp - (uint64_t)offset->v]);
    } else {
        
    }
}

void _GLOAD(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    tint_t *addr;

    if (r1->t == INT) {
        addr = (tint_t *)r1;
        dlvm_push(vm, vm->stack->stack[(uint64_t)addr->v]);
    } else {
        
    }
}

void dlvm_exec(dlvm_t *vm) {
    opcode_t opcode;
    ttype_t *r1, *r2, *res;
    uint8_t gc = 0;

    while (1) {
        opcode = dlvm_next_op(vm);
        switch(opcode) {
            case ADD:
                _ADD(vm);
                break;
            case MUL:
                _MUL(vm);
                break;
            case SUB:
                _SUB(vm);
                break;
            case DIV:
                _DIV(vm);
                break;
            case MOD:
                _MOD(vm);
                break;
            case AND:
                _AND(vm);
                break;
            case OR:
                _OR(vm);
                break;
            case NOT:
                _NOT(vm);
                break;
            case PUSH:
                _PUSH(vm);
                break;
            case POP:
                _POP(vm);
                break;
            case LOAD:
                _LOAD(vm);
                break;
            case GLOAD:
                _GLOAD(vm);
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
                dlvm_gc_run(vm);
                return;
            default:
                return;
        }

        if (check_and_print_error(vm)) {
            return;
        }

        if (!(++gc)) {
            dlvm_gc_run(vm);
        }
    }
}