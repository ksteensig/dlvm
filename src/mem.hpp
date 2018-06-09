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
  Result<Error, addr_t> operator()(paddr_t paddr) {
    auto [addr, allocated] = paddr;
    if (allocated > offset + 1) {
      return ReturnError<addr_t>(SEGMENTATION_FAULT, "");
    } else {
      return ReturnOk<>(addr + offset);
    }
  }
};

class MemoryManager {
  addr_t m_max_pagetable = 8192;
  addr_t m_pagetable_alloc = 8192;
  addr_t m_heap_alloc = 8192;
  addr_t m_max_heap = 8192;
  addr_t m_max_stack = 4096;
  addr_t m_stack_alloc = 4096;

  addr_t heap_ptr = 0;
  addr_t stack_ptr = 0;

  unique_ptr<optional<paddr_t>[]> PageTable;
  unique_ptr<ReferenceType[]> Heap;
  unique_ptr<ValueType[]> Stack;

  Result<Error, paddr_t> TranslatePAddress(addr_t addr);

  // return memory freed
  void GarbageCollect();

 public:
  MemoryManager() {}

  Result<Error, ValueType> Insert(addr_t addr, uint32_t offset,
                                  ValueType value);
  Result<Error, ValueType> Insert(addr_t addr, ValueType value);
  Result<Error, ValueType> Access(addr_t addr, uint32_t offset);
  Result<Error, ValueType> Access(addr_t addr);

  Result<Error, addr_t> Malloc(uint32_t size);

  Result<Error, ValueType> Push(ValueType value);
  Result<Error, ValueType> Pop();
};

}  // namespace dlvm
