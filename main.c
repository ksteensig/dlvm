#include "src/vm.h"

int main(int argc, char* argv[])
{
    uint64_t prog[] = {0x03, 1, 0x03, 1, 0x01, 0xFF};

    stack_t *stack = stack_init(100);
    program_t *program = malloc(sizeof(program_t));
    program->program = prog;
    program->program_size = 6;

    dlvm_t *vm = dlvm_init(program, stack);

    dlvm_exec(vm);

    return 0;
}