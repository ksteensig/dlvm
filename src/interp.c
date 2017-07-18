#include "interp.h"

uint8_t dlvm_next_op(dlvm_t *vm) {
    return vm->program[vm->pc++];
}

void dlvm_push(dlvm_t *vm, ttype_t *o) {
    static ttype_t *last_pushed;

    if (last_pushed != NULL) {
         o->next = last_pushed;
    }
    
    last_pushed = o;

    vm->stack[vm->sp++] = o;
}

ttype_t *dlvm_pop(dlvm_t *vm) {
    ttype_t *ret = vm->stack[--(vm->sp)];
    vm->stack[vm->sp + 1] = NULL;

    return ret;
}

/*
bool check_and_print_error(dlvm_t *vm) {
    ttype_t *typ = vm->stack[vm->sp - 1];
    terror_t *err;

    printf("%d\n", typ->t);

    if (typ->t == ERROR) {
        err = (terror_t *)typ;
        printf(err->msg);
        return true;
    }

    return false;
}
*/

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

ttype_t *parse_int(vm) {
	int64_t v = dlvm_next_op(vm);

	for (uint8_t i = 0; i < 7; i++) {
		v = (v << 8);
		v = v | dlvm_next_op(vm);
	}

	return (ttype_t *)init_int(v);
}

void _PUSH(dlvm_t *vm) {
    ttype_t *r1;
    uint8_t type = dlvm_next_op(vm);

    switch (type) {
        case BOOL:
            r1 = init_int((bool)dlvm_next_op(vm));
            break;
        case INT:
            //r1 = init_int(dlvm_next_op(vm));
			r1 = parse_int(vm);
            break;
        case FLOAT:
            r1 = init_float((double)dlvm_next_op(vm));
            break;
/*
        case STRING:
            r1 = init_char((char)dlvm_next_op(vm));
            break;
*/
        case LIST:
            r1 = init_list();
            break;
        case FUNCTION:
            // first arg is argc and second arg is addr
            r1 = init_fun(dlvm_next_op(vm), dlvm_next_op(vm));
            break;
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
        dlvm_push(vm, vm->stack[vm->fp - (uint64_t)offset->v]);
    } else {
        
    }
}

void _GLOAD(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    tint_t *addr;

    if (r1->t == INT) {
        addr = (tint_t *)r1;
        dlvm_push(vm, vm->stack[(uint64_t)addr->v]);
    } else {
        
    }
}

void _INSERT_LIST(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    ttype_t *r2 = dlvm_pop(vm);
    ttype_t *res = insert_into_list(r1, r2, dlvm_next_op(vm));
    dlvm_push(vm, res);
}

void _SET_LIST(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    ttype_t *r2 = dlvm_pop(vm);
    ttype_t *res = overwrite_list(r1, r2, dlvm_next_op(vm));
    dlvm_push(vm, res);
}

void _ACCESS_LIST(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    ttype_t *res = access_list(r1, dlvm_next_op(vm));
    dlvm_push(vm, res);
}

void _INVOKE(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    dlvm_push(vm, init_int(((tfun_t *)r1)->argc));
    dlvm_push(vm, init_int(vm->fp));
    dlvm_push(vm, init_int(vm->pc));
    vm->fp = vm->sp;
    vm->pc = ((tfun_t *)r1)->addr;
}

void _CALL(dlvm_t *vm) {
    dlvm_push(vm, init_int(dlvm_next_op(vm))); // push argc to stack
    dlvm_push(vm, init_int(vm->fp));
    dlvm_push(vm, init_int(vm->pc));
    vm->fp = vm->sp;
    vm->pc = dlvm_next_op(vm); // addr
}

void _RET(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm); // get result
    vm->sp = vm->fp;
    vm->pc = (uint64_t)((tint_t *)dlvm_pop(vm))->v;
    vm->fp = (uint64_t)((tint_t *)dlvm_pop(vm))->v;
    vm->sp -= (uint64_t)((tint_t *)dlvm_pop(vm))->v;
    dlvm_push(vm, r1); // push result back to stack
}

void _JMP(dlvm_t *vm) {
    uint64_t addr = dlvm_next_op(vm);

    vm->pc = addr;
}

void _JMPF(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    uint64_t addr = dlvm_next_op(vm);

    if (r1 == NULL) {
        return;
    }

    if (r1->t == BOOL && !((tbool_t *)r1)->b) {
        vm->pc = addr;
    } else {
        
    }
}

void _JMPT(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    uint64_t addr = dlvm_next_op(vm);

    if (r1 == NULL) {
        return;
    }

    if (r1->t == BOOL && ((tbool_t *)r1)->b) {
        vm->pc = addr;
    } else {
        
    }
}

void _CMP_EQ(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    ttype_t *r2 = dlvm_pop(vm);

    if (r1 == NULL || r2 == NULL) {
        //dlvm_push(vm, init_error(""));
        return;
    }

    if (is_number(r1) && is_number(r2)) {
        dlvm_push(vm, equals(r1, r2));
        return;
    }

    if (r1->t != r2->t) {
        //dlvm_push(vm, init_error(""));
        return;
    }

    switch (r1->t) {
        case BOOL:
            dlvm_push(vm, bool_equals((tbool_t *)r1, (tbool_t *)r2));
            break;
        case STRING:
            dlvm_push(vm, string_equals((tstring_t *)r1, (tstring_t *)r2));
            break;
        default:
			return;
            //dlvm_push(vm, init_error(""));
    }
}

void _CMP_L(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    ttype_t *r2 = dlvm_pop(vm);

    if (r1 == NULL || r2 == NULL) {
        //dlvm_push(vm, init_error(""));
        return;
    }

    if (is_number(r1) && is_number(r2)) {
        dlvm_push(vm, less_than(r1, r2));
    }

    //dlvm_push(vm, init_error(""));
}

void _CMP_G(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    ttype_t *r2 = dlvm_pop(vm);

    if (r1 == NULL || r2 == NULL) {
        //dlvm_push(vm, init_error(""));
        return;
    }

    if (is_number(r1) && is_number(r2)) {
        dlvm_push(vm, greater_than(r1, r2));
    }

    //dlvm_push(vm, init_error(""));
}

void _CMP_LE(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    ttype_t *r2 = dlvm_pop(vm);

    if (r1 == NULL || r2 == NULL) {
        //dlvm_push(vm, init_error(""));
        return;
    }

    if (is_number(r1) && is_number(r2)) {
        return dlvm_push(vm, and(equals(r1, r2), less_than(r1, r2)));
    }

    //dlvm_push(vm, init_error(""));
}

void _CMP_GE(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);
    ttype_t *r2 = dlvm_pop(vm);

    if (r1 == NULL || r2 == NULL) {
        //dlvm_push(vm, init_error(""));
        return;
    }

    if (is_number(r1) && is_number(r2)) {
        return dlvm_push(vm, and(equals(r1, r2), greater_than(r1, r2)));
    }

    //dlvm_push(vm, init_error(""));
}

/*
void print_list(dlvm_t *vm, tlist_t *list, tlist_t *known) {
    append_list(known, list);

    for (uint64_t i = 0; i < list->elements; i++) {
        switch ((list->list[i])->t) {
            case LIST:
                if (list->list[i] == list) {
                    printf(", [...]");
                }
        }
    }
}

void print_primitive(dlvm_t *vm, ttype_t *r1) {
    switch(r1->t) {
        case NIL:
            printf("NIL");
            break;
        case BOOL:
            printf("%s", ((tbool_t *)r1)->v ? "TRUE" : "FALSE");
            break;
        case CHAR:
            printf("%c", ((tstring_t *)r1)->v);
            break;
        case INT:
            printf("%ld", ((tint_t *)r1)->v);
            break;
        case FLOAT:
            printf("%lf", ((tfloat_t *)r1)->v);
            break;
        default:
            return;
    }
}

void _PRINT_LIST(dlvm_t *vm) {
    ttype_t *r1 = dlvm_pop(vm);

    if (r1 == NULL) {
        return;
    }

    switch(r1->t) {
        case LIST:
            print_list(vm, (tlist_t *)r1);
            break;
        default:
            print_primitive(vm, r1, (tlist_t *)init_list());
    }
}

void _PRINT_LIST(dlvm_t *vm) {

}

void _PRINT_STRING(dlvm_t *vm) {

}
*/

void _PRINT(dlvm_t *vm) {

}

void dlvm_exec_header() {
	uint8_t bytecode;
}

void dlvm_parse_header(dlvm_module_t *module) {

}

void dlvm_exec(dlvm_t *vm) {
    uint8_t bytecode;
    uint8_t gc = 0;

    while (1) {
        bytecode = dlvm_next_op(vm);
		printf("bytecode: %d\n", bytecode);
        switch(bytecode) {
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
                _INSERT_LIST(vm);
                break;
            case SET_LIST:
                _SET_LIST(vm);
                break;
            case ACCESS_LIST:
                _ACCESS_LIST(vm);
                break;
            case INVOKE: // invoke function object
                _INVOKE(vm);
                break;
            case CALL: // invoke procedure
                _CALL(vm);
                break;
            case RET:
                _RET(vm);
                break;
            case JMP:
                _JMP(vm);
                break;
            case JMPF:
                _JMPF(vm);
                break;
            case JMPT:
                _JMPT(vm);
                break;
            case CMP_EQ:
                _CMP_EQ(vm);
                break;
            case CMP_L:
                _CMP_L(vm);
                break;
            case CMP_G:
                _CMP_G(vm);
                break;
            case CMP_LE:
                _CMP_LE(vm);
                break;
            case CMP_GE:
                _CMP_GE(vm);
                break;
            case PRINT:
                _PRINT(vm);
                break;
            case HALT:
                dlvm_gc_run(vm);
                return;
            default:
                return;
        }
        /*
        if (check_and_print_error(vm)) {
            return;
        }
        */

        if (!(++gc)) {
            dlvm_gc_run(vm);
        }
    }
}
