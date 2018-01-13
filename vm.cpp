#include "vm.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

Type VM::Pop() {
    auto obj = Stack->back();
    Stack->pop_back();
    SP--;
    return obj;
}

void VM::Push(Type obj) {
    Stack->push_back(obj);
    SP++;
}

uint8_t VM::Next() {
    return Program->at(PC++);
}

uint64_t VM::GetQuad() {
    uint64_t value = 0;

    for (uint8_t i = 0; i < 8; i++) {
        value = (value << 8) | Next();
    }

    return value;
}

void VM::GarbageCollect() {

}

Result<addr_t> VM::Malloc(uint32_t size) {
    auto addr = 0;

    if (size + HP > hsize) {
        GarbageCollect();
        if (size + HP > hsize)
            return Result{(addr_t)0, "Out of heap memory.", OUT_OF_MEMORY};
    }

    for (auto i = 0; i < PageTable->size(); i++) {
        if (!PageTable->at(i).has_value()) {
            PageTable->at(i) = optional<addr_t>{HP};
            HP += size;
            return Result{(addr_t)i, "", OK};
        }
    }

    return Result{(addr_t)0, "Unknown run-time error.", UNKNOWN};
}

Result<addr_t> VM::TranslateAddress(addr_t addr) {
    addr_t a;

    if (PageTable->at(addr).has_value()) {
        a = PageTable->at(addr).value();
        return Result{(addr_t)a, "", OK};
    }

    return Result{(addr_t)0, "Trying to access an invalid address", NULL_REFERENCE};
}

Result<Type> VM::POP_HANDLER() {
    if (Stack->size() > 0) {
        Pop();
        return OkResult<Type>(NIL, (bool)false);
    }

    return Result<Type>{Type{NIL, (addr_t)0}, "Out of bounds on stack", OUT_OF_BOUNDS};
}

Result<Type> VM::PUSH_INT_HANDLER() {
    auto value = GetQuad();
    Push(Type{INTEGER, (int64_t)value});
    cout << "hi" << endl;
    return OkResult<Type>(INTEGER, (int64_t)value);
}

Result<Type> VM::ADD_HANDLER() {
    auto t1 = Pop();
    auto t2 = Pop();

    Result<Type> res = t1 + t2;

    if (res.ErrCode == OK) {
        Push(res.Value);
    }

    return res;
}

void VM::PRINT_HANDLER() {
    cout << Pop() << endl;
}

Result<Type> VM::CREATE_REFERENCE_HANDLER() {
    auto pt = Malloc(1);
    auto obj = Pop();

    if (pt.ErrCode != OK) {
        //return mem;
    }

    auto hp = TranslateAddress(pt.Value);

    if (hp.ErrCode != OK) {

    }

    Heap[hp.Value] = obj;
    auto ref = Type{REFERENCE, pt.Value};
    Push(ref);
    
    return Result<Type>{ref, "", OK};
}

}