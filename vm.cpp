#include "vm.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

unique_ptr<Type> VM::Pop() {
    auto obj = move(Stack.back());
    Stack.pop_back();
    SP--;
    return obj;
}

void VM::Push(unique_ptr<Type> obj) {
    Stack.push_back(move(obj));
    SP++;
}

uint8_t VM::Next() {
    return Program.at(PC++);
}
}