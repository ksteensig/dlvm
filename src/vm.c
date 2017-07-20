#include "vm.h"

void dlvm_parse_header(dlvm_module_t *module) {
	uint64_t file_size;
	uint8_t header_size;

	FILE *mf = fopen(module->path->utf8.str, "rb");

	fseek(mf, 0L, SEEK_END);
	file_size = ftell(mf);
	rewind(mf);

	if (file_size < 8) {
		return;
	}

	uint8_t temp_file_size[8];
	fread(temp_file_size, sizeof(uint8_t), 8, mf);
	
	//file_size = (uint64_t) temp_
/*
	for (uint8_t i = 0; i < 7; i++) {

	}
*/
}

dlvm_module_t *dlvm_module_init(uint8_t *path) {
	dlvm_module_t *module = malloc(sizeof(dlvm_module_t));
	module->path = convert_to_utf8(path, strlen(path) + 1);

	return module;	
}

dlvm_t *dlvm_init(uint8_t *p, uint64_t ps, uint64_t ss) {
    dlvm_t *vm = malloc(sizeof(dlvm_t));

    vm->program = malloc(sizeof(ttype_t *) * ps);
    vm->program_size = ps;

	memcpy(vm->program, p, ps);

	vm->stack = malloc(sizeof(ttype_t *) * ss);
	vm->stack_size = ss;

    vm->pc = 0;
    vm->sp = 0;
    vm->fp = 0;

    return vm;
}

uint8_t dlvm_next_op(dlvm_t *vm) {
    return vm->program[vm->pc++];
}

void dlvm_push(dlvm_t *vm, ttype_t *o) {
    static ttype_t *last_pushed;

    if (last_pushed != NULL) {
		o->next = last_pushed;
    }
    
    last_pushed = o;

    vm->stack[vm->sp++] = o;
}

ttype_t *dlvm_pop(dlvm_t *vm) {
    ttype_t *ret = vm->stack[--(vm->sp)];
    vm->stack[vm->sp + 1] = NULL;

    return ret;
}

uint64_t dlvm_take_8bytes(dlvm_t *vm) {
	uint64_t v = dlvm_next_op(vm);

	for (uint8_t i = 0; i < 7; i++) {
		v = v << 8;
		v = v | dlvm_next_op(vm);
	}

	return v;
}
