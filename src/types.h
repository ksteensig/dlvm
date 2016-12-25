#ifndef LISP_LANG_TYPES_H
#define LISP_LANG_TYPES_H

typedef enum type_e {
    NIL         = 0,
    BOOL        = 1,
    INT         = 2,
    FLOAT       = 3,
    STRING      = 4,
    ARRAY       = 5,
    FUNCTION    = 6
} type_t;

typedef struct array_s {
    uint32_t len;
    void *arr;
} array_t;

typedef struct val_s {
    type_t t;
    void *val;
} val_t;

#endif //LISP_LANG_TYPES_H
