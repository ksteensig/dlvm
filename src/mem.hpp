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

struct BoundsCheck {
  uint32_t offset;
  BoundsCheck(uint32_t offset) : offset{offset} {}
  Result<addr_t> operator()(paddr_t paddr) {
    if (get<1>(paddr) > offset + 1) {
      return ReturnError<addr_t>(SEGMENTATION_FAULT, "");
    } else {
      return ReturnOk(get<0>(paddr) + offset);
    }
  }
};

class HeapManager {
  addr_t m_heap_alloc = 8192;
  addr_t m_max_heap = 8192;
  addr_t m_max_pagetable = 8192;
  addr_t m_pagetable_alloc = 8192;
  addr_t heap_ptr = 0;

  unique_ptr<ReferenceType[]> Heap;
  unique_ptr<optional<paddr_t>[]> PageTable;

  Result<paddr_t> TranslatePAddress(addr_t addr);

 public:
  HeapManager()
      : Heap{make_unique<ReferenceType[]>(m_max_heap)},
        PageTable{make_unique<optional<paddr_t>[]>(m_max_pagetable)} {}

  Result<ValueType> Insert(addr_t addr, uint32_t offset, ValueType value);
  Result<ValueType> Insert(addr_t addr, ValueType value);
  Result<ValueType> Access(addr_t addr, uint32_t offset);
  Result<ValueType> Access(addr_t addr);

  Result<addr_t> Malloc(uint32_t size);
};

class StackManager {
  addr_t m_max_stack = 4096;
  addr_t m_stack_alloc = 4096;

  unique_ptr<ValueType[]> Stack;

 public:
  StackManager() : Stack{make_unique<ValueType[]>(m_max_stack)} {}

  Result<ValueType> Push(ValueType value);
  Result<ValueType> Pop();
};

}  // namespace dlvm
