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
} ttype_t;

typedef struct tnil_s {
    type_t t;
} tnil_t;

typedef struct tbool_s {
    type_t t;
    uint8_t v;
} tbool_t;

typedef struct tint_s {
    type_t t;
    int32_t v;
} tint_t;

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

