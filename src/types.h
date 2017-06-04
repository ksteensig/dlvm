#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum type_e {
    NIL         = 0,
    BOOL        = 1,
    INT         = 2,
    FLOAT       = 3,
    STRING      = 4,
    LIST        = 5,
    FUNCTION    = 6
} type_t;

/* type to convert to */
typedef struct ttype_s {
    type_t t;
    struct ttype_s *next;
    char marked;
} ttype_t;

typedef struct tint_s {
    type_t t;
    ttype_t *next;
    char marked;

    int64_t v;
} tint_t;

ttype_t *init_int(int64_t v);
void free_int(ttype_t *r1);
ttype_t *add_int(ttype_t *r1, ttype_t *r2);

/*
typedef struct tnil_s {
    type_t t;
} tnil_t;

typedef struct tbool_s {
    type_t t;
    uint8_t v;
} tbool_t;

typedef struct tfloat_s {
    type_t t;
    float v;
} tfloat_t;

typedef struct tstring_s {
    type_t t;
    char *v;
} tstr_t;

typedef struct tlist_s {
    type_t t;
    intptr_t *v;
} tlist_t;

typedef struct tfun_s {
    type_t t;
    int fargs;
    intptr_t addr;
    intptr_t *env;
} tfun_t;
*/
