#include "types.h"

ttype_t *init_int(int64_t v) {
    tint_t *res = malloc(sizeof(tint_t));
    res->v = v;
    res->t = INT;
    res->marked = 0;

    return (ttype_t *)res;
}

void free_int(ttype_t *r1) {
    free((tint_t *)r1);
}

ttype_t *init_float(double v) {
    tfloat_t *res = malloc(sizeof(tfloat_t));
    res->v = v;
    res->t = FLOAT;
    res->marked = 0;

    return (ttype_t *)res;
}

void free_float(ttype_t *r1) {
    free((tfloat_t *)r1);
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

ttype_t *add(ttype_t *r1, ttype_t *r2) {
    if (!is_number(r1) || !is_number(r2)) {
        return init_error("adding one or two non-numbers\n");
    } else if (is_float(r1) && is_float(r2)) {
        return init_float(((tfloat_t *)r1)->v + ((tfloat_t *)r2)->v);
    } else if (is_int(r1) && is_int(r2)) {
        return init_int(((tint_t *)r1)->v + ((tint_t *)r2)->v);
    } else if (is_int(r1) && is_float(r2)) {
        return init_float(((tint_t *)r1)->v + ((tfloat_t *)r2)->v);
    } else if (is_float(r1) && is_int(r2)) {
        return init_float(((tfloat_t *)r1)->v + ((tint_t *)r2)->v);
    }

    return init_error("something happened while adding\n");
}

ttype_t *subtract(ttype_t *r1, ttype_t *r2) {
    if (!is_number(r1) || !is_number(r2)) {
        return init_error("subtracting one or two non-numbers\n");
    } else if (is_float(r1) && is_float(r2)) {
        return init_float(((tfloat_t *)r1)->v - ((tfloat_t *)r2)->v);
    } else if (is_int(r1) && is_int(r2)) {
        return init_int(((tint_t *)r1)->v - ((tint_t *)r2)->v);
    } else if (is_int(r1) && is_float(r2)) {
        return init_float(((tint_t *)r1)->v - ((tfloat_t *)r2)->v);
    } else if (is_float(r1) && is_int(r2)) {
        return init_float(((tfloat_t *)r1)->v - ((tint_t *)r2)->v);
    }

    return init_error("something happened while subtracting\n");
}

ttype_t *multiply(ttype_t *r1, ttype_t *r2) {
    if (!is_number(r1) || !is_number(r2)) {
        return init_error("multiplying one or two non-numbers\n");
    } else if (is_float(r1) && is_float(r2)) {
        return init_float(((tfloat_t *)r1)->v * ((tfloat_t *)r2)->v);
    } else if (is_int(r1) && is_int(r2)) {
        return init_int(((tint_t *)r1)->v * ((tint_t *)r2)->v);
    } else if (is_int(r1) && is_float(r2)) {
        return init_float(((tint_t *)r1)->v * ((tfloat_t *)r2)->v);
    } else if (is_float(r1) && is_int(r2)) {
        return init_float(((tfloat_t *)r1)->v * ((tint_t *)r2)->v);
    }

    return init_error("something happened while multiplying\n");
}

ttype_t *divide(ttype_t *r1, ttype_t *r2) {
    if (!is_number(r1) || !is_number(r2)) {
        return init_error("dividing one or two non-numbers\n");
    } else if (is_float(r1) && is_float(r2)) {
        return init_float(((tfloat_t *)r1)->v / ((tfloat_t *)r2)->v);
    } else if (is_int(r1) && is_int(r2)) {
        return init_int(((tint_t *)r1)->v / ((tint_t *)r2)->v);
    } else if (is_int(r1) && is_float(r2)) {
        return init_float(((tint_t *)r1)->v / ((tfloat_t *)r2)->v);
    } else if (is_float(r1) && is_int(r2)) {
        return init_float(((tfloat_t *)r1)->v / ((tint_t *)r2)->v);
    }

    return init_error("something happened while dividing\n");
}

ttype_t *modulo(ttype_t *r1, ttype_t *r2) {
    if (!is_int(r1) || !is_int(r2)) {
        return init_error("applying modulo to one or two non-integers\n");
    } else if (is_int(r1) && is_int(r2)) {
        return init_int(((tint_t *)r1)->v % ((tint_t *)r2)->v);
    }

    return init_error("something happened while dividing\n");
}

ttype_t *init_error(char *msg) {
    terror_t *err = malloc(sizeof(terror_t));
    err->t = ERROR;
    err->marked = 0;
    err->msg = msg;

    return (ttype_t *)err;
}