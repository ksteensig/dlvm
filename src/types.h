#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum type_e {
    NIL         = 0,
    BOOL        = 1,
    INT         = 2,
    FLOAT       = 3,
    CHAR        = 4,
    LIST        = 5,
    FUNCTION    = 6,
    ERROR       = 0xFF,
} type_t;

/* type to convert to */
typedef struct ttype_s {
    type_t t;
    struct ttype_s *next;
    char marked;
} ttype_t;

typedef struct tbool_s {
    type_t t;
    ttype_t *next;
    char marked;

    bool v;
} tbool_t;

typedef struct tint_s {
    type_t t;
    ttype_t *next;
    char marked;

    int64_t v;
} tint_t;

typedef struct tfloat_s {
    type_t t;
    ttype_t *next;
    char marked;

    double v;
} tfloat_t;

typedef struct tchar_s {
    type_t t;
    ttype_t *next;
    char marked;

    char v;
} tchar_t;

typedef struct tlist_s {
    type_t t;
    ttype_t *next;
    char marked;

    ttype_t *list;
    uint64_t arr_len;
    uint64_t last_elem;
} tlist_t;

typedef struct tfun_s {
    type_t t;
    struct ttype_s *next;
    char marked;

    int argc;
    intptr_t addr;
    intptr_t *env;
} tfun_t;

typedef struct terror_s {
    type_t t;
    ttype_t *next;
    char marked;

    char *msg;
} terror_t;

ttype_t *add(ttype_t *r1, ttype_t *r2);
ttype_t *subtract(ttype_t *r1, ttype_t *r2);
ttype_t *multiply(ttype_t *r1, ttype_t *r2);
ttype_t *divide(ttype_t *r1, ttype_t *r2);
ttype_t *modulo(ttype_t *r1, ttype_t *r2);

bool is_number(ttype_t *r);
bool is_float(ttype_t *r);
bool is_int(ttype_t *r);

ttype_t *init_int(int64_t v);
void free_int(ttype_t *r1);

ttype_t *init_float(double v);
void free_float(ttype_t *r1);

ttype_t *init_error(char *msg);