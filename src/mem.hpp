#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

using allocated_t = uint32_t;
using paddr_t = pair<addr_t, allocated_t>;

class MemoryManager {
  addr_t m_max_pagetable = 8192;
  addr_t m_pagetable_alloc = 8192;
  addr_t m_heap_alloc = 8192;
  addr_t m_max_heap = 8192;
  addr_t m_max_stack = 4096;
  addr_t m_stack_alloc = 4096;

  addr_t heap_ptr = 0;
  addr_t stack_ptr = 0;

  shared_ptr<optional<paddr_t>[]> PageTable;
  shared_ptr<ReferenceType[]> Heap;
  shared_ptr<ValueType[]> Stack;

  Result<paddr_t> TranslatePAddress(addr_t addr);

  struct PageTableBoundsCheck;
  struct HeapBoundsCheck;

  // return memory freed, take memory needed
  Result<uint32_t> GarbageCollect(uint32_t needed);
  // return memory freed
  Result<uint32_t> GarbageCollect();

  Result<void> OnError(Error e);

 public:
  MemoryManager() {}

  Result<ValueType> Insert(ValueType addr, ValueType offset, ValueType value);

  Result<ValueType> Insert(addr_t addr, uint32_t offset, ValueType value);

  Result<ValueType> Access(ValueType addr, ValueType offset);

  Result<ValueType> Access(addr_t addr, uint32_t offset);

  Result<ValueType> Malloc(ValueType size);

  Result<addr_t> Malloc(uint32_t size);

  Result<ValueType> Push(ValueType value);
  Result<ValueType> Pop();
};

struct MemoryManager::PageTableBoundsCheck {
  uint32_t offset;
  PageTableBoundsCheck(uint32_t offset) : offset{offset} {}
  Result<addr_t> operator()(paddr_t paddr) {
    auto [addr, allocated] = paddr;
    if (allocated > offset + 1) {
      return ReturnError<addr_t>(SEGMENTATION_FAULT, "");
    } else {
      return ReturnOk<>(addr + offset);
    }
  }
};

struct MemoryManager::HeapBoundsCheck {
  uint32_t max_heap;
  uint32_t heap_ptr;
  HeapBoundsCheck(uint32_t max_heap, uint32_t heap_ptr)
      : max_heap{max_heap}, heap_ptr{heap_ptr} {}
  Result<uint32_t> operator()(uint32_t offset) {
    if (max_heap > heap_ptr + offset) {
      return ReturnError<uint32_t>(OUT_OF_MEMORY, "Cannot allocate Heap");

    } else {
      return ReturnOk<uint32_t>(offset);
    }
  }
};

}  // namespace dlvm
