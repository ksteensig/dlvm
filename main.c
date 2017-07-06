#include "src/vm.h"

int main(int argc, char* argv[])
{
    uint64_t prog[] = {PUSH, INT, 10, PUSH, INT, 2, MUL, PRINT, HALT};

    stack_t *stack = stack_init(100);
    program_t *program = malloc(sizeof(program_t));
    program->program = prog;
    program->program_size = 6;

    dlvm_t *vm = dlvm_init(program, stack);

    dlvm_exec(vm);

    return 0;
}