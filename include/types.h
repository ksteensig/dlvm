#pragma once

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
    STRING      = 4,
    LIST        = 5,
    FUNCTION    = 6,
    EXCEPTION   = 0xFF,
} type_t;

/* type to convert to */
typedef struct ttype_s {
    type_t t;
    struct ttype_s *next;
    bool marked;
} ttype_t;

typedef struct tbool_s {
    type_t t;
    ttype_t *next;
    bool marked;

    bool b;
} tbool_t;

typedef struct tint_s {
    type_t t;
    ttype_t *next;
    bool marked;

    int64_t v;
} tint_t;

typedef struct tfloat_s {
    type_t t;
    ttype_t *next;
    bool marked;

    double v;
} tfloat_t;

typedef struct tstring_s {
    type_t t;
    ttype_t *next;
    bool marked;

    utf8_t str;
} tstring_t;

typedef struct tlist_s {
    type_t t;
    ttype_t *next;
    bool marked;

    ttype_t **list;
    uint64_t length;
    uint64_t elements;
} tlist_t;

/* procedure type */
typedef struct tfun_s {
    type_t t;
    ttype_t *next;
    bool marked;

	uint64_t call_count;
    uint64_t argc;
    uint64_t addr;
} tfun_t;

/* exception type */
typedef struct ex_s {
    type_t t;
    ttype_t *next;
    bool marked;

    uint64_t stack_pos;
    uint8_t stack_range;

    uint64_t opcode_pos;
    uint8_t opcode_range;
    
    char *msg;
} ex_t;

typedef struct ex_handler_s {
	type_t t;
	ttype_t *next;
	bool marked;

	ex_handler_t *prev;
} ex_handler_t

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

ttype_t *add_ints(tint_t *r1, tint_t *r2);
ttype_t *subtract_ints(tint_t *r1, tint_t *r2);
ttype_t *multiply_ints(tint_t *r1, tint_t *r2);
ttype_t *divide_ints(tint_t *r1, tint_t *r2);
ttype_t *modulo_ints(tint_t *r1, tint_t *r2);

tfloat_t *int_to_float(tint_t *r1);

ttype_t *add_floats(tfloat_t *r1, tfloat_t *r2);
ttype_t *subtract_floats(tfloat_t *r1, tfloat_t *r2);
ttype_t *multiply_floats(tfloat_t *r1, tfloat_t *r2);
ttype_t *divide_floats(tfloat_t *r1, tfloat_t *r2);

// external arithmetic API that does the typechecking
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

ttype_t *bool_equals(tbool_t *r1, tbool_t *r2);
ttype_t *int_equals(tint_t *r1, tint_t *r2);
ttype_t *float_equals(tfloat_t *r1, tfloat_t *r2);
ttype_t *char_equals(tstring_t *r1, tstring_t *r2);

ttype_t *equals(ttype_t *r1, ttype_t *r2);
ttype_t *less_than(ttype_t *r1, ttype_t *r2);
ttype_t *greater_than(ttype_t *r1, ttype_t *r2);

ttype_t *int_less_than(tint_t *r1, tint_t *r2);
ttype_t *int_greater_than(tint_t *r1, tint_t *r2);
ttype_t *float_less_than(tfloat_t *r1, tfloat_t *r2);
ttype_t *float_greater_than(tfloat_t *r1, tfloat_t *r2);

void print_nil();
void print_bool(tbool_t *r1);
void print_int(tint_t *r1);
void print_float(tfloat_t *r1);
void print_char(tstring_t *r1);
void print_primitive(ttype_t *r1);
void print_list(tlist_t *r1, tlist_t *known_lists);
