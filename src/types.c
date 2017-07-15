#include "types.h"

ttype_t *init_bool(bool b) {
    tbool_t *res = malloc(sizeof(tbool_t));
    res->b = b;
    res->t = BOOL;
    res->marked = false;
    res->next = NULL;

    return (ttype_t *)res;
}

void free_bool(ttype_t *r1) {
    free((tbool_t *)r1);
}

ttype_t *init_int(int64_t v) {
    tint_t *res = malloc(sizeof(tint_t));
    res->v = v;
    res->t = INT;
    res->marked = false;
    res->next = NULL;

    return (ttype_t *)res;
}

void free_int(ttype_t *r1) {
    free((tint_t *)r1);
}

ttype_t *init_float(double v) {
    tfloat_t *res = malloc(sizeof(tfloat_t));
    res->v = v;
    res->t = FLOAT;
    res->marked = false;
    res->next = NULL;

    return (ttype_t *)res;
}

void free_float(ttype_t *r1) {
    free((tfloat_t *)r1);
}

ttype_t *init_string(utf8_t *s) {
    tstring_t *res = malloc(sizeof(tstring_t));
    res->str = s;
    res->t = STRING;
    res->marked = false;
    res->next = NULL;

    return (ttype_t *)res;
}

void free_string(ttype_t *o) {
    free((tstring_t *)r1);
}

ttype_t *init_list() {
    tlist_t *res = malloc(sizeof(tlist_t));
    res->t = LIST;
    res->marked = false;
    res->next = NULL;

    res->list = malloc(sizeof(ttype_t *) * 2);
    res->arr_len = 2;
    res->elements = 0;

    return (ttype_t *)res;
}

void free_list(ttype_t *list) {
    free(((tlist_t *)list)->list);
    free((tlist_t *)list);
}

ttype_t *init_fun(uint64_t argc, uint64_t addr) {
    tfun_t *fun = malloc(sizeof(tfun_t));
    fun->t = FUNCTION;
    fun->marked = false;
    fun->next = NULL;

	fun->calls = 0;
    fun->addr = addr;
    fun->argc = argc;

    return (ttype_t *)fun;
}

void free_fun(ttype_t *fun) {
    free((tfun_t *)fun);
}

ttype_t *init_exception(exception_type_t ex_type, uint64_t err-code, utf8_t *msg) {
    terror_t *ex = malloc(sizeof(terror_t));
    ex->t = EXCEPTION;
    ex->marked = false;
    ex->next = NULL;

    ex->msg = msg;
	ex->error_type = ex_type;
	ex->error_code = err_code;

    return (ttype_t *)ex;
}

void free_exception(ttype_t *ex) {
    free(((terror_t *)err)->msg);
    free((terror_t *)err);
}

bool is_float(ttype_t *r) {
    return (r->t == FLOAT);
}

bool is_int(ttype_t *r) {
    return (r->t == INT);
}

bool is_number(ttype_t *r) {
    return (is_int(r) || is_float(r));
}

ttype_t *add_ints(tint_t *r1, tint_t *r2) {
    return (ttype_t *)init_int(r1->v + r2->v);
}

ttype_t *subtract_ints(tint_t *r1, tint_t *r2) {
    return (ttype_t *)init_int(r1->v - r2->v);
}

ttype_t *multiply_ints(tint_t *r1, tint_t *r2) {
    return (ttype_t *)init_int(r1->v * r2->v);
}

ttype_t *divide_ints(tint_t *r1, tint_t *r2) {
    return (ttype_t *)init_int(r1->v / r2->v);
}

ttype_t *modulo_ints(tint_t *r1, tint_t *r2) {
    return (ttype_t *)init_int(r1->v % r2->v);
}

tfloat_t *int_to_float(tint_t *r1) {
    return (tfloat_t *)init_float(r1->v);
}

ttype_t *add_floats(tfloat_t *r1, tfloat_t *r2) {
    return (ttype_t *)init_float(r1->v + r2->v);
}

ttype_t *subtract_floats(tfloat_t *r1, tfloat_t *r2) {
    return (ttype_t *)init_float(r1->v - r2->v);
}

ttype_t *multiply_floats(tfloat_t *r1, tfloat_t *r2) {
    return (ttype_t *)init_float(r1->v * r2->v);
}

ttype_t *divide_floats(tfloat_t *r1, tfloat_t *r2) {
    return (ttype_t *)init_float(r1->v / r2->v);
}

ttype_t *add(ttype_t *r1, ttype_t *r2) {
    if (!is_number(r1) || !is_number(r2)) {
        return init_error("adding one or two non-numbers\n");
    } else if (is_float(r1) && is_float(r2)) {
        return add_floats((tfloat_t *)r1, (tfloat_t *)r2);
    } else if (is_int(r1) && is_int(r2)) {
        return add_ints((tint_t *)r1, (tint_t *)r2);
    } else if (is_int(r1) && is_float(r2)) {
        return add_floats(int_to_float((tint_t *)r1), (tfloat_t *)r2);
    } else if (is_float(r1) && is_int(r2)) {
        return add_floats((tfloat_t *)r1, int_to_float((tint_t *)r2));
    }

    return init_error("something happened while adding\n");
}

ttype_t *subtract(ttype_t *r1, ttype_t *r2) {
    if (!is_number(r1) || !is_number(r2)) {
        return init_error("subtracting one or two non-numbers\n");
    } else if (is_float(r1) && is_float(r2)) {
        return subtract_floats((tfloat_t *)r1, (tfloat_t *)r2);
    } else if (is_int(r1) && is_int(r2)) {
        return subtract_ints((tint_t *)r1, (tint_t *)r2);
    } else if (is_int(r1) && is_float(r2)) {
        return subtract_floats(int_to_float((tint_t *)r1), (tfloat_t *)r2);
    } else if (is_float(r1) && is_int(r2)) {
        return subtract_floats((tfloat_t *)r1, int_to_float((tint_t *)r2));
    }

    return init_error("something happened while subtracting\n");
}

ttype_t *multiply(ttype_t *r1, ttype_t *r2) {
    if (!is_number(r1) || !is_number(r2)) {
        return init_error("multiplying one or two non-numbers\n");
    } else if (is_float(r1) && is_float(r2)) {
        return multiply_floats((tfloat_t *)r1, (tfloat_t *)r2);
    } else if (is_int(r1) && is_int(r2)) {
        return multiply_ints((tint_t *)r1, (tint_t *)r2);
    } else if (is_int(r1) && is_float(r2)) {
        return multiply_floats(int_to_float((tint_t *)r1), (tfloat_t *)r2);
    } else if (is_float(r1) && is_int(r2)) {
        return multiply_floats((tfloat_t *)r1, int_to_float((tint_t *)r2));
    }

    return init_error("something happened while multiplying\n");
}

ttype_t *divide(ttype_t *r1, ttype_t *r2) {
    if (!is_number(r1) || !is_number(r2)) {
        return init_error("dividing one or two non-numbers\n");
    } else if (is_float(r1) && is_float(r2)) {
        return divide_floats((tfloat_t *)r1, (tfloat_t *)r2);
    } else if (is_int(r1) && is_int(r2)) {
        return divide_ints((tint_t *)r1, (tint_t *)r2);
    } else if (is_int(r1) && is_float(r2)) {
        return divide_floats(int_to_float((tint_t *)r1), (tfloat_t *)r2);
    } else if (is_float(r1) && is_int(r2)) {
        return divide_floats((tfloat_t *)r1, int_to_float((tint_t *)r2));
    }

    return init_error("something happened while dividing\n");
}

ttype_t *modulo(ttype_t *r1, ttype_t *r2) {
    if (!is_int(r1) || !is_int(r2)) {
        return init_error("applying modulo to one or two non-integers\n");
    } else if (is_int(r1) && is_int(r2)) {
        return modulo_ints((tint_t *)r1, (tint_t *)r2);
    }

    return init_error("something happened while dividing\n");
}

bool is_bool(ttype_t *r) {
    return (r->t == BOOL);
}

ttype_t *and(ttype_t *r1, ttype_t *r2) {
    if (!is_bool(r1) || !is_bool(r2)) {
        return NULL;
    } else {
        return init_bool(((tbool_t *)r1)->v && ((tbool_t *)r2)->v);
    }
}

ttype_t *or(ttype_t *r1, ttype_t *r2) {
    if (!is_bool(r1) || !is_bool(r2)) {
        return NULL;
    } else {
        return init_bool(((tbool_t *)r1)->v || ((tbool_t *)r2)->v);
    }
}

ttype_t *not(ttype_t *r) {
    if (!is_bool(r)) {
        return NULL;
    } else {
        return init_bool(!((tbool_t *)r)->v);
    }
}

ttype_t *append_list(ttype_t *list, ttype_t *obj) {
    tlist_t *l = (tlist_t *)list;
    return insert_into_list(list, obj, l->elements+1);
}

ttype_t *prepend_list(ttype_t *list, ttype_t *obj) {
    return insert_into_list(list, obj, 0);
}

ttype_t *insert_into_list(ttype_t *list, ttype_t *obj, uint64_t pos) {
    tlist_t *l = (tlist_t *)list;

    if (l->elements == UINT64_MAX) {
        return init_error("list reached maximum capacity\n");
    } else if (pos > l->elements + 1) {
        return init_error("out of bounds error while inserting into list\n");
    } else if (l->elements == l->arr_len) {
        ttype_t *res = grow_list(l);

        switch (res->t) {
            case ERROR:
                return res;
            default:
                l = (tlist_t *)res;
        }
    }

    ttype_t **n_list = malloc(sizeof(ttype_t *) * l->arr_len);
    n_list[pos] = obj;
    memcpy(n_list[0], l->list[0], sizeof(ttype_t *) * pos);
    memcpy(n_list[pos+1], l->list[pos], sizeof(ttype_t *) * l->elements - pos);

    free(l->list);

    l->list = n_list;

    return (ttype_t *)l;
}

ttype_t *overwrite_list(ttype_t *list, ttype_t *obj, uint64_t pos) {
    tlist_t *l = (tlist_t *)list;

    if (pos > l->elements + 1) {
        return init_error("out of bounds error while overwriting position in list\n");
    }
    
    l->list[pos] = obj;

    return (ttype_t *)l;
}

ttype_t *access_list(ttype_t *list, uint64_t pos) {
    tlist_t *l = (tlist_t *)list;

    if (pos > l->elements + 1) {
        return init_error("out of bounds error while accessing list\n");
    }

    return l->list[pos];
}

ttype_t *grow_list(tlist_t *list) {
    tlist_t *l = (tlist_t *)list;

    if (l->elements == UINT64_MAX) {
        return init_error("list reached maximum capacity\n");
    }

    ttype_t **n_list = malloc(sizeof(ttype_t *) * l->arr_len * 2);
    memcpy(n_list[0], l->list[0], sizeof(ttype_t *) * l->elements);
   
    free(l->list);

    l->list = n_list;
    l->arr_len *= 2;

    return (ttype_t *)l;
}

ttype_t *bool_equals(tbool_t *r1, tbool_t *r2) {
    if (r1->v == r2->v) {
        return init_bool(true);
    } else {
        return init_bool(false);
    }
}

ttype_t *int_equals(tint_t *r1, tint_t *r2) {
    if (r1->v == r2->v) {
        return init_bool(true);
    } else {
        return init_bool(false);
    }
}

ttype_t *float_equals(tfloat_t *r1, tfloat_t *r2) {
    if (r1->v == r2->v) {
        return init_bool(true);
    } else {
        return init_bool(false);
    }
}

ttype_t *char_equals(tstring_t *r1, tstring_t *r2) {
    if (r1->v == r2->v) {
        return init_bool(true);
    } else {
        return init_bool(false);
    }
}

ttype_t *less_than(ttype_t *r1, ttype_t *r2) {
    if (is_int(r1) && is_int(r2)) {
        return int_less_than((tint_t *)r1, (tint_t *)r2);
    } else if (is_float(r1) && is_float(r2)) {
        return float_less_than((tfloat_t *)r1, (tfloat_t *)r2);
    } else if (is_int(r1) && is_float(r2)) {
        return float_less_than(int_to_float(r1), (tfloat_t *)r2);
    } else if (is_float(r1) && is_int(r2)) {
        return float_less_than((tfloat_t *)r1, int_to_float(r2));
    }

    return init_error("");
}

ttype_t *greater_than(ttype_t *r1, ttype_t *r2) {
    if (is_int(r1) && is_int(r2)) {
        return int_greater_than((tint_t *)r1, (tint_t *)r2);
    } else if (is_float(r1) && is_float(r2)) {
        return float_greater_than((tfloat_t *)r1, (tfloat_t *)r2);
    } else if (is_int(r1) && is_float(r2)) {
        return float_greater_than(int_to_float(r1), (tfloat_t *)r2);
    } else if (is_float(r1) && is_int(r2)) {
        return float_greater_than((tfloat_t *)r1, int_to_float(r2));
    }

    return init_error("");
}

ttype_t *equals(ttype_t *r1, ttype_t *r2) {
    if (is_int(r1) && is_int(r2)) {
        return int_equals((tint_t *)r1, (tint_t *)r2);
    } else if (is_float(r1) && is_float(r2)) {
        return float_equals((tfloat_t *)r1, (tfloat_t *)r2);
    } else if (is_int(r1) && is_float(r2)) {
        return float_equals(int_to_float(r1), (tfloat_t *)r2);
    } else if (is_float(r1) && is_int(r2)) {
        return float_equals((tfloat_t *)r1, int_to_float(r2));
    }

    return init_error("");
}

ttype_t *int_less_than(tint_t *r1, tint_t *r2) {
    return init_bool(r1->v < r2->v);
}

ttype_t *int_greater_than(tint_t *r1, tint_t *r2) {
    return init_bool(r1->v > r2->v);
}

ttype_t *float_less_than(tfloat_t *r1, tfloat_t *r2) {
    return init_bool(r1->v < r2->v);
}

ttype_t *float_greater_than(tfloat_t *r1, tfloat_t *r2) {
    return init_bool(r1->v > r2->v);
}

void print_nil() {
    printf("NIL");
}

void print_bool(tbool_t *r1) {
    printf("%s", r1->v ? "TRUE" : "FALSE");
}

void print_int(tint_t *r1) {
    printf("%ld", ((tint_t *)r1)->v);
}

void print_float(tfloat_t *r1) {
    printf("%lf", ((tfloat_t *)r1)->v);
}

void print_char(tstring_t *r1) {
    
}

void print_primitive(ttype_t *r1) {
    switch(r1->t) {
        case NIL:
            print_nil();
            break;
        case BOOL:
            print_bool((tbool_t *)r1);
            break;
        case CHAR:
            print_char((tstring_t *)r1);
            break;
        case INT:
            print_int((tint_t *)r1);
            break;
        case FLOAT:
            print_float((tfloat_t *)r1);
            break;
        default:
            return;
    }
}

void print_list(tlist_t *r1, tlist_t *known_lists) {

}
