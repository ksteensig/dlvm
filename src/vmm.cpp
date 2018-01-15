#include "vmm.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

Result<ValueType> Memory::Pop() {
    if (stack_ptr > 0) {
        return OkResult(
            Stack[--stack_ptr]
        );
    } else {
        return ThrowError<ValueType>("Out of bounds on the stack", OUT_OF_BOUNDS);
    }
}

Result<ValueType> Memory::Push(ValueType obj) {
    if (stack_ptr == m_stack_alloc) {

    }

    Stack[stack_ptr++] = obj;
    return OkResult(obj);
}

inline vaddr_t Memory::TranslateAddress(addr_t addr) {
    return PageTable[addr];
}

inline bool Memory::OccupiedAddress(vaddr_t addr) {
    return (addr >> 0x3F) == 1;
}

Result<ValueType> Memory::Malloc(uint32_t size) {
    if (size + heap_ptr > m_heap_alloc) {
        //GarbageCollect();
        if (size + heap_ptr > m_heap_alloc)
            return ThrowError<ValueType>("Out of heap memory.", OUT_OF_MEMORY);
    }

    for (addr_t i = 0; i < m_pagetable_alloc; i++) {
        if (!OccupiedAddress(PageTable[i])) {
            PageTable[i] = (1 << 64) | heap_ptr;
            heap_ptr += size;
            return OkResult(ValueType{REFERENCE, i});
        }
    }

    return ThrowError<ValueType>("Out of address space", OUT_OF_MEMORY);
}

void Memory::Move(addr_t src, addr_t dest, uint32_t size) {
    copy_n(&(Heap[src]), size, &(Heap[dest]));
}

void Memory::Reset(addr_t dest, uint32_t size) {
    for (addr_t i = dest; i < dest + size; i++) {
        Heap[i] = ReferenceType{};
    }
}

inline uint32_t Memory::Mark(addr_t addr) {
    uint32_t marked = 0;
    addr_t heap_addr = (addr_t)PageTable[addr];

    if (Heap[heap_addr].Marked) {
        return marked;
    }

    switch (Heap[heap_addr].type) {
        case STRUCT:
        case STRING:
        case ARRAY: {
            Heap[heap_addr].Marked = true;
            marked++;
            auto size = get<array_t>(Heap[heap_addr].Value);
            for (addr_t i = heap_addr + 1; i < heap_addr + size;) {
                if (Heap[heap_addr].type == REFERENCE) {
                    marked += Mark(get<addr_t>(Stack[i].Value));
                }
            }
        }
            break;
        case REFERENCE:
            Heap[heap_addr].Marked = true;
            marked++;
            marked += Mark(get<addr_t>(Heap[heap_addr].Value));
            break;
        default:
            Heap[heap_addr].Marked = true;
            marked++;
    }

    return marked;
}

inline void Memory::Compact(uint32_t marked) {
    uint32_t moved = 0;

    for (addr_t i = 0; i < heap_ptr; i++) {
        if (moved == marked) {
            break;
        } else if (Heap[i].Marked) {
            moved++;
        } else {
            /*
            switch (Heap[i].type) {
                case ARRAY:
                default {
                    Heap[i]
                }
            }
            */
        }
    }
}

void Memory::GarbageCollect() {
    uint32_t marked = 0;

    for (addr_t i = 0; i < m_stack_alloc; i++) {
        if (Stack[i].type == REFERENCE) {
            marked += Mark(get<addr_t>(Stack[i].Value));
        }
    }

    Compact(marked);
}

}