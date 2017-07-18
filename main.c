#include "interp.h"
#include "opcodes.h"

int main(int argc, char* argv[])
{
    uint64_t prog[] = {
        PUSH, INT, 10,
        PUSH, INT, 2,
        MUL,
        PRINT,
        PUSH, STRING, '\n',
        PRINT,
        HALT
    };

    dlvm_t *vm = dlvm_init(prog, 9, 100);

    dlvm_exec(vm);

    return 0;
}
