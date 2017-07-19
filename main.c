#include "interp.h"
#include "opcodes.h"

int main(int argc, char* argv[])
{
    uint8_t prog[1000] = {
        PUSH, INT, 0, 0, 0, 0, 0, 0, 0, 2,
        PUSH, INT, 0, 0, 0, 0, 0, 0, 0, 10,
        ADD,
        PRINT,
        //PUSH, STRING, '\n',
        PRINT,
        HALT
    };

    dlvm_t *vm = dlvm_init(prog, 1000, 100);

    dlvm_exec(vm);

    return 0;
}
