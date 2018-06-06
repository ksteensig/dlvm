#include "vmm.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

Result<ValueType> MemoryManager::Pop() {
  if (stack_ptr > 0) {
    return LiftType(Stack[--stack_ptr]);
  } else {
    return LiftError<ValueType>(OUT_OF_BOUNDS, "Out of bounds on stack");
  }
}

Result<ValueType> MemoryManager::Push(ValueType obj) {
  if (stack_ptr == m_stack_alloc) {
    return LiftError<ValueType>(OUT_OF_MEMORY, "");
  }

  Stack[stack_ptr++] = obj;
  return LiftType(obj);
}

inline vaddr_t MemoryManager::TranslateAddress(addr_t addr) {
  return PageTable[addr];
}

inline bool MemoryManager::OccupiedAddress(vaddr_t addr) {
  return (addr >> 0x3F) == 1;
}

Result<ValueType> MemoryManager::Malloc(uint32_t size) {
  if (size + heap_ptr > m_heap_alloc) {
    // GarbageCollect();
    if (size + heap_ptr > m_heap_alloc)
      return LiftError<ValueType>(OUT_OF_MEMORY, "Out of heap memory.");
  }

  for (addr_t i = 0; i < m_pagetable_alloc; i++) {
    if (!OccupiedAddress(PageTable[i])) {
      PageTable[i] = (1 << 64) | heap_ptr;
      heap_ptr += size;
      return LiftType(ValueType{PTR, i});
    }
  }

  return LiftError<ValueType>(OUT_OF_MEMORY, "Out of address space");
}

void MemoryManager::Copy(addr_t src, addr_t dest, uint32_t size) {
  copy_n(&(Heap[src]), size, &(Heap[dest]));
}

void MemoryManager::Reset(addr_t dest, uint32_t size) {
  for (addr_t i = dest; i < dest + size; i++) {
    Heap[i] = ReferenceType{};
  }
}

void MemoryManager::UpdatePageTable(addr_t old_addr, addr_t new_addr) {
  for (addr_t i = 0; i < m_pagetable_alloc; i++) {
    if (old_addr == PageTable[i]) {
      PageTable[i] = PageTable[i] | (1 << 0x40) | new_addr;
    }
  }
}

void MemoryManager::ResetPageTable() {
  for (addr_t i = 0; i < m_pagetable_alloc; i++) {
    PageTable[i] &= ~(1 << 0x40);
  }
}

addr_t MemoryManager::Move(addr_t dest) {
  addr_t next_dead;

  for (addr_t src = dest; src < heap_ptr; src++) {
    if (Heap[src].Marked) {
      switch (Heap[src].type) {
        case ARRAY: {
          array_t size{get<array_t>(Heap[src].Value)};
          Copy(src, dest, size);
          Reset(dest, size);
          UpdatePageTable(src, dest);
          next_dead = dest + size;
        } break;
        default:
          Copy(src, dest, 1);
          Reset(dest, 1);
          UpdatePageTable(src, dest);
          next_dead = dest + 1;
          break;
      }
    }
  }

  return next_dead;
}

inline uint32_t MemoryManager::Mark(addr_t addr) {
  uint32_t marked = 0;
  addr_t heap_addr = (addr_t)PageTable[addr];

  if (Heap[heap_addr].Marked) {
    return marked;
  }

  switch (Heap[heap_addr].type) {
    case STRING:
    case ARRAY: {
      Heap[heap_addr].Marked = true;
      marked++;
      auto size = get<array_t>(Heap[heap_addr].Value);
      for (addr_t i = heap_addr + 1; i < heap_addr + size;) {
        if (Heap[heap_addr].type == PTR) {
          marked += Mark(get<addr_t>(Stack[i].Value));
        }
      }
    } break;
    case PTR:
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

inline void MemoryManager::Compact(uint32_t marked) {
  uint32_t moved = 0;

  for (addr_t i = 0; i < heap_ptr; i++) {
    if (moved == marked) {
      heap_ptr = i;
      break;
    } else if (Heap[i].Marked) {
      moved++;
      Heap[i].Marked = false;
      switch (Heap[i].type) {
        case ARRAY:
          i += get<array_t>(Heap[i].Value);
          break;
        default:
          break;
      }
    } else {
      moved++;
      auto new_dead = Move(i);
      Heap[i].Marked = false;
      i = new_dead;
    }
  }
}

void MemoryManager::GarbageCollect() {
  uint32_t marked = 0;

  for (addr_t i = 0; i < m_stack_alloc; i++) {
    if (Stack[i].type == PTR) {
      marked += Mark(get<addr_t>(Stack[i].Value));
    }
  }

  ResetPageTable();

  Compact(marked);
}

inline Result<ReferenceType> MemoryManager::Insert(addr_t addr, uint32_t offset,
                                                   ReferenceType value) {
  auto vaddr = TranslateAddress(addr);
}

inline Result<ReferenceType> MemoryManager::Access(addr_t addr,
                                                   uint32_t offset) {}

}  // namespace dlvm
