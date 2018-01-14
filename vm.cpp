#include "vm.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

Result VM::Pop() {
    if (Stack->size() > 0) {
        auto obj = Stack->back();
        Stack->pop_back();
        SP--;
        return OkResult(obj);
    }
}

Result VM::Push(Type obj) {
    Stack->push_back(obj);
    SP++;

    return OkResult(Type{});
}

uint8_t VM::Next() {
    return Program->at(PC++);
}

uint64_t VM::GetEightBytes() {
    uint64_t value = 0;

    for (uint8_t i = 0; i < 8; i++) {
        value = (value << 8) | Next();
    }

    return value;
}

uint32_t VM::Mark(addr_t addr) {
    auto marked = 0;
    switch (Heap[addr].type) {
        case INTEGER:
            Heap[addr].Marked = true;
            marked++;
    }

    return marked;
}

uint32_t VM::Move(addr_t heap_addr) {
    auto data_addr = 0;
    auto delta = 0;
    auto moved = 0;

    for (addr_t i = 0; i < HP; i++) {
        if (Heap[i].Marked) {
            data_addr = i;
            switch (Heap[i].type) {
                case INTEGER:
                    Heap[heap_addr] = Heap[i];
                    Heap[i].Marked = false;
                    delta = i - heap_addr;
                    moved = 0;
                    break;
                default:
                    return 0;
            }
        }
    }

    for (addr_t i = 0; i < PageTable->size(); i++) {
        if (PageTable->at(i).has_value()) {
            if (PageTable->at(i).value() == data_addr) {
                PageTable->at(i) = optional<addr_t>{data_addr};
                break;
            }
        }
    }

    return moved;
}

void VM::Compact(uint32_t marked) {
    auto relocated = 0;

    for (addr_t i = 0; i < HP; i++) {
        if (relocated == marked) {

        }
    }

}

void VM::GarbageCollect() {
    auto marked = 0;

    for (auto o: *Stack) {
        if (o.type == REFERENCE) {
            marked += Mark(get<addr_t>(o.Value));
            break;
        }
    }

    Compact(marked);
}

Result VM::Malloc(uint32_t size) {
    if (size + HP > hsize) {
        GarbageCollect();
        if (size + HP > hsize)
            return ThrowError("Out of heap memory.", OUT_OF_MEMORY);
    }

    for (uint32_t i = 0; i < PageTable->size(); i++) {
        if (!PageTable->at(i).has_value()) {
            PageTable->at(i) = optional<addr_t>{HP};
            HP += size;
            return OkResult(Type{REFERENCE, (addr_t)i});
        }
    }

    return ThrowError("Out of Address space", OUT_OF_MEMORY);
}

Result VM::TranslateAddress(addr_t addr) {
    auto heap_addr = PageTable->at(addr);

    if (!heap_addr.has_value()) {
        return ThrowError("Null reference", NULL_REFERENCE);
    }

    return OkResult(Type{REFERENCE, (addr_t)heap_addr.value()});
}

Result VM::POP_HANDLER() {
    if (Stack->size() > 0) {
        Pop();
        return OkResult(Type{});
    }

    return ThrowError("Out of bounds on stack", OUT_OF_BOUNDS);
}

Result VM::PUSH_INT_HANDLER() {
    auto value = GetEightBytes();
    auto res = Push(Type{INTEGER, (int64_t)value});
    if (res.ErrCode == OK) {
        return OkResult(res.Value);
    }

    return res;
}

Result VM::ADD_HANDLER() {
    auto t1 = Pop();
    auto t2 = Pop();

    Result res = t1 + t2;

    if (res.ErrCode == OK) {
        Push(res.Value);
    }

    return res;
}

Result VM::PRINT_HANDLER() {
    auto obj = Pop();

    if (obj.ErrCode != OK) {
        
    }

    cout << obj.Value << endl;

    return OkResult(obj.Value);
}

Result VM::CREATE_REFERENCE_HANDLER() {
    auto page_addr = Malloc(1);

    if (page_addr.ErrCode != OK) {

    }

    auto page = get<addr_t>(page_addr.Value.Value);

    auto obj = Pop();

    if (obj.ErrCode != OK) {

    }

    auto heap_addr = TranslateAddress(page);

    if (heap_addr.ErrCode != OK) {

    }

    auto heap = get<addr_t>(heap_addr.Value.Value);

    if (last_in_heap.has_value()) {
        auto addr = TranslateAddress(last_in_heap.value());
        if (addr.ErrCode == OK) {
            Heap[get<addr_t>(addr.Value.Value)].Next = page;
            last_in_heap = optional<addr_t>{heap};
        }
    }

    Heap[heap] = obj.Value;
    Type ref{REFERENCE, page};
    Push(ref);
    
    return OkResult(Type{});
}

}