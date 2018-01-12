#include "vm.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

shared_ptr<Type> VM::Pop() {
    auto obj = Stack.back();
    Stack.pop_back();
    SP--;
    return obj;
}

void VM::Push(shared_ptr<Type> obj) {
    Stack.push_back(obj);
    SP++;
}

uint8_t VM::Next() {
    return Program->at(PC++);
}

void VM::POP_HANDLER() {
    Pop();
}

void VM::PUSH_INT_HANDLER() {
    uint64_t value = 0;
    uint8_t next = 0;

    for (uint8_t i = 0; i < 8; i++) {
        next = Next();
        value = (value << 8) | next;
    }

    Push(make_shared<TInteger>((int64_t)value));
}

void VM::PRINT_HANDLER() {
    auto obj = Pop();

    obj->Print();
}

void VM::ADD_HANDLER() {
    auto lhs = Pop();
    auto rhs = Pop();

    return

    
}

}