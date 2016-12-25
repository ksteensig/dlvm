#include "vm.h"

int exec(vm_t *vm)
{
    while (1)
    {
        uint32_t opcode = NCODE(vm);

        int a, b, rval;
        uint32_t addr, argc;

        switch(opcode)
        {
            case NOP:
                break;
            case ADD:
                a = POP(vm);
                b = POP(vm);
                PUSH(vm, a+b);
            case MUL:
                a = POP(vm);
                b = POP(vm);
                PUSH(vm, a*b);
                break;
            case POP:
                vm->sp--;
                break;
            case PUSH:
                a = NCODE(vm);
                PUSH(vm, a);
                break;
            case CALL:
                addr = NCODE(vm);
                argc = NCODE(vm);
                PUSH(vm, argc);
                PUSH(vm, vm->fp);
                PUSH(vm, vm->pc);
                vm->fp = vm->sp;
                vm->pc = addr;
                break;
            case RET:
                rval = POP(vm);
                vm->sp = vm->fp;
                vm->pc = POP(vm);
                vm->fp = POP(vm);
                argc = POP(vm);
                vm->sp -= argc;
                PUSH(vm, rval);
            case PRINT:
                a = POP(vm);
                printf("%d\n", a);
                break;
            case HALT:
                return 0;
            default:
                return 1;
        }
    }
}