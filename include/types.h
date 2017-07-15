#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TYPE_HEADER	type_t t; type_t *next; bool marked;

typedef enum type_e {
    NIL         = 0,
    BOOL        = 1,
    INT         = 2,
    FLOAT       = 3,
    STRING      = 4,
    LIST        = 5,
    FUNCTION    = 6,
    ERROR       = 0xFF,
} type_t;

typedef enum exception_type_e {
	IO				0x00,
	MEMORY			0x01,
	ARITHMETIC		0x02,
	INSTRUCTION		0x03,
	TYPE			0x04,
	INVALID_DATA	0x05,
	RUNTIME			0x06,
	CUSTOM			0xFF
} exception_type_t;

/* type to convert to */
typedef struct theader_s {
	TYPE_HEADER
} theader_t;

typedef struct tbool_s {
	TYPE_HEADER;
    bool b;
} tbool_t;

typedef struct tint_s {
	TYPE_HEADER;
    int64_t v;
} tint_t;

typedef struct tfloat_s {
	TYPE_HEADER;
    double v;
} tfloat_t;

typedef struct tstring_s {
	TYPE_HEADER;
    utf8_t *str;
} tstring_t;

typedef struct tlist_s {
	TYPE_HEADER;
    ttype_t **list;
    uint64_t arr_len;
    uint64_t elements;
} tlist_t;

typedef struct tfun_s {
	TYPE_HEADER;
	uint64_t calls;
    uint64_t argc;
    uint64_t addr;
} tfun_t;

typedef struct texception_s {
	TYPE_HEADER;
	exception_type_t error_type;
	uint16_t error_code;
    utf8_t *msg;
} texception_t;

typedef struct texception_handler_s {
	TYPE_HEADER;
	texception_handler_t *prev;
	tfun_t *handler;
} texception_handler_t;

ttype_t *init_bool(bool b);
void free_bool(ttype_t *o);

ttype_t *init_int(int64_t v);
void free_int(ttype_t *o);

ttype_t *init_float(double v);
void free_float(ttype_t *o);

ttype_t *init_string(utf8_t *s);
void free_string(ttype_t *o);

ttype_t *init_list();
void free_list(ttype_t *o);

ttype_t *init_fun(uint64_t argc, uint64_t addr);
void free_fun(ttype_t *o);

ttype_t *init_exception(exception_type_t type, uint16_t error, utf8_t *msg);
void free_exception(ttype_t *o);

inline bool is_null(ttype_t *o);
inline bool is_char(ttype_t *o);
inline bool is_number(ttype_t *o);
inline bool is_float(ttype_t *o);
inline bool is_int(ttype_t *o);
inline bool is_list(ttype_t *o);
inline bool is_function(ttype_t *o);
inline bool is_exception(ttype_t *o);

ttype_t *add_ints(tint_t *v1, tint_t *v2);
ttype_t *subtract_ints(tint_t *v1, tint_t *v2);
ttype_t *multiply_ints(tint_t *v1, tint_t *v2);
ttype_t *divide_ints(tint_t *v1, tint_t *v2);
ttype_t *modulo_ints(tint_t *v1, tint_t *v2);

tfloat_t *int_to_float(tint_t *v1);

ttype_t *add_floats(tfloat_t *v1, tfloat_t *v2);
ttype_t *subtract_floats(tfloat_t *v1, tfloat_t *v2);
ttype_t *multiply_floats(tfloat_t *v1, tfloat_t *v2);
ttype_t *divide_floats(tfloat_t *v1, tfloat_t *v2);

// external arithmetic API that does the typechecking
ttype_t *add(ttype_t *o1, ttype_t *o2);
ttype_t *subtract(ttype_t *o1, ttype_t *o2);
ttype_t *multiply(ttype_t *o1, ttype_t *o2);
ttype_t *divide(ttype_t *o1, ttype_t *o2);
ttype_t *modulo(ttype_t *o1, ttype_t *o2);


ttype_t *and(ttype_t *o1, ttype_t *o2);
ttype_t *or(ttype_t *o1, ttype_t *o2);
ttype_t *not(ttype_t *o);

// append object into list
ttype_t *append_list(ttype_t *list, ttype_t *o);

// prepend object into list
ttype_t *prepend_list(ttype_t *list, ttype_t *o);

// insert between objects in a list
ttype_t *insert_into_list(ttype_t *list, ttype_t *o, uint64_t pos);

// overwrite a position in a list, there has to be an existing object
ttype_t *overwrite_list(ttype_t *list, ttype_t *o, uint64_t pos);

// access an element of a list
ttype_t *access_list(ttype_t *list, uint64_t pos);

// internal api to grow the list as it's implemented as a flex array
ttype_t *grow_list(tlist_t *list);

ttype_t *bool_equals(tbool_t *b1, tbool_t *b2);
ttype_t *int_equals(tint_t *v1, tint_t *v2);
ttype_t *float_equals(tfloat_t *v1, tfloat_t *v2);
ttype_t *string_equals(tstring_t *s1, tstring_t *s2);

ttype_t *equals(ttype_t *o1, ttype_t *o2);
ttype_t *less_than(ttype_t *o1, ttype_t *o2);
ttype_t *greater_than(ttype_t *o1, ttype_t *o2);

ttype_t *int_less_than(tint_t *o1, tint_t *o2);
ttype_t *int_greater_than(tint_t *o1, tint_t *o2);
ttype_t *float_less_than(tfloat_t *o1, tfloat_t *o2);
ttype_t *float_greater_than(tfloat_t *o1, tfloat_t *o2);

void print_nil();
void print_bool(tbool_t *b);
void print_int(tint_t *v);
void print_float(tfloat_t *v);
void print_string(tstring_t *s);
void print_primitive(ttype_t *o);
void print_list(tlist_t *list, tlist_t *known_lists);
