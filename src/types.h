#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
#pragma pack(4)
typedef struct ttype_s {
    type_t t;
    struct ttype_s *next;
    bool marked;
} ttype_t;

#pragma pack(4)
typedef struct tbool_s {
    type_t t;
    ttype_t *next;
    bool marked;

    bool v;
} tbool_t;

#pragma pack(4)
typedef struct tint_s {
    type_t t;
    ttype_t *next;
    bool marked;

    int64_t v;
} tint_t;

#pragma pack(4)
typedef struct tfloat_s {
    type_t t;
    ttype_t *next;
    bool marked;

    double v;
} tfloat_t;

#pragma pack(4)
typedef struct tchar_s {
    type_t t;
    ttype_t *next;
    bool marked;

    char v;
} tchar_t;

#pragma pack(4)
typedef struct tlist_s {
    type_t t;
    ttype_t *next;
    bool marked;

    ttype_t **list;
    uint64_t arr_len;
    uint64_t elements;
} tlist_t;

#pragma pack(4)
typedef struct tfun_s {
    type_t t;
    struct ttype_s *next;
    bool marked;

    uint64_t argc;
    uint64_t addr;
} tfun_t;

#pragma pack(4)
typedef struct terror_s {
    type_t t;
    ttype_t *next;
    bool marked;

    char *msg;
} terror_t;

ttype_t *init_bool(bool v);
void free_bool(ttype_t *r1);

ttype_t *init_int(int64_t v);
void free_int(ttype_t *r1);

ttype_t *init_float(double v);
void free_float(ttype_t *r1);

ttype_t *init_char(char v);
void free_char(ttype_t *r1);

ttype_t *init_list();
void free_list(ttype_t *list);

ttype_t *init_fun(uint64_t argc, uint64_t addr);
void free_fun(ttype_t *fun);

ttype_t *init_error(char *msg);
void free_error(ttype_t *err);

bool is_number(ttype_t *r);
bool is_float(ttype_t *r);
bool is_int(ttype_t *r);

ttype_t *add(ttype_t *r1, ttype_t *r2);
ttype_t *subtract(ttype_t *r1, ttype_t *r2);
ttype_t *multiply(ttype_t *r1, ttype_t *r2);
ttype_t *divide(ttype_t *r1, ttype_t *r2);
ttype_t *modulo(ttype_t *r1, ttype_t *r2);

bool is_bool(ttype_t *r);

ttype_t *and(ttype_t *r1, ttype_t *r2);
ttype_t *or(ttype_t *r1, ttype_t *r2);
ttype_t *not(ttype_t *r);

// append object into list
ttype_t *append_list(ttype_t *list, ttype_t *obj);

// prepend object into list
ttype_t *prepend_list(ttype_t *list, ttype_t *obj);

// insert between objects in a list
ttype_t *insert_into_list(ttype_t *list, ttype_t *obj, uint64_t pos);

// overwrite a position in a list, there has to be an existing object
ttype_t *overwrite_list(ttype_t *list, ttype_t *obj, uint64_t pos);

// access an element of a list
ttype_t *access_list(ttype_t *list, uint64_t pos);

// internal api to grow the list as it's implemented as a flex array
ttype_t *grow_list(tlist_t *list);