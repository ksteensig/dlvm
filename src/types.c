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

ttype_t *add_int(ttype_t *r1, ttype_t *r2) {
    ttype_t *res;
    res = init_int(((tint_t *)r1)->v + ((tint_t *)r2)->v);

    return res;
}