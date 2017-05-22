#include "vm.h"

dlvm_t *dlvm_init(program_t *p, stack_t *s, heap_t *h) {
    dlvm_t *vm = malloc(sizeof(dlvm_t));

    vm->program = p;
    vm->stack = s;
    vm->heap = h;

    return vm;
}

program_t *program_init(char *filename) {
    program_t *prog = malloc(sizeof(program_t));

    return prog;
}

stack_t *stack_init(uint64_t stack_size) {
    stack_t *stack = malloc(sizeof(stack_t));

    stack->stack_size = stack_size;
    stack->stack = malloc(sizeof(uint64_t) * stack_size);

    return stack;
}

heap_t *heap_init(uint64_t heap_size) {
    heap_t *heap = malloc(sizeof(heap_t));

    heap->heap_size = heap_size;
    heap->heap = malloc(sizeof(uint64_t) * heap_size);

    heap->heap_objs_count = 0;

    return heap;
}

uint64_t dlvm_next_op(dlvm_t *vm) {
    return vm->program[++(vm->pc)];
}

uint64_t dlvm_next_op(dlvm_t *vm) {
    return vm->program[++(vm->pc)];
}

/*
uint32_t VM::Next()
{
    return Program[++pc];
}

intptr_t VM::Pop()
{
    return Stack[sp--];
}

void VM::Push(intptr_t v)
{
    Stack[++sp] = v;
}

void VM::Exec()
{
    intptr_t a, b, rval;
    intptr_t addr, argc;
    uint32_t opcode;

    while (1)
    {
        opcode = Next();

        switch(opcode)
        {
            case NOP:
                break;
            case ADD:
                a = Pop();
                b = Pop();
                Push(a+b);
            case MUL:
                a = Pop();
                b = Pop();
                Push(a*b);
                break;
            case POP:
                Pop();
                break;
            case PUSH:
                a = Next();
                Push(a);
                break;
            case CALL:
                addr = Next();
                argc = Next();
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
*/