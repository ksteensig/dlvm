#pragma once

#include "vm.h"

void dlvm_gc_mark(dlvm_t *vm);

void dlvm_gc_sweep(dlvm_t *vm);

void dlvm_gc_reset_marked(dlvm_t *vm);

void dlvm_gc_run(dlvm_t *vm);