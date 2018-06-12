#include "mem.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

Result<Error, paddr_t> MemoryManager::TranslatePAddress(addr_t addr) {
  if (addr > m_max_pagetable) {
    return ReturnError<paddr_t>(SEGMENTATION_FAULT, "");
  } else {
    auto paddr = PageTable[addr];
    if (paddr.has_value()) {
      return ReturnOk(paddr.value());
    } else {
      return ReturnError<paddr_t>(SEGMENTATION_FAULT, "");
    }
  }
}

Result<Error, ValueType> MemoryManager::Insert(addr_t addr, uint32_t offset,
                                               ValueType value) {
  function<Result<Error, ValueType>(addr_t)> insert =
      [this, value](addr_t addr) -> Result<Error, ValueType> {
    Heap[addr] = value;
    return ReturnOk<ValueType>(value);
  };

  return TranslatePAddress(addr)
      .RightMap(static_cast<function<Result<Error, addr_t>(paddr_t)>>(
          PageTableBoundsCheck{offset}))
      .RightMap(insert);
}

Result<Error, ValueType> MemoryManager::Access(addr_t addr, uint32_t offset) {
  return TranslatePAddress(addr)
      .RightMap(static_cast<function<Result<Error, addr_t>(paddr_t)>>(
          PageTableBoundsCheck{offset}))
      .RightMap(static_cast<function<Result<Error, ValueType>(addr_t)>>(
          [this](addr_t addr) { return this->Heap[addr].Unbox(); }));
}

Result<Error, addr_t> MemoryManager::Malloc(uint32_t size) {
  function<Result<Error, uint32_t>(uint32_t)> bounds_check =
      [this](uint32_t size) {
        return HeapBoundsCheck{this->m_max_heap, this->heap_ptr}(size);
      };

  function<Result<Error, uint32_t>(Error)> gc = [this](Error _) {
    return this->GarbageCollect();
  };

  function<Result<Error, addr_t>(uint32_t)> allocate = [this](uint32_t size) {
    for (addr_t i = 0; i < m_max_pagetable; i++) {
      if (!this->PageTable[i].has_value()) {
        this->PageTable[i] = make_optional(make_pair(i, size));
        return ReturnOk<addr_t>(i);
      }
    }
    return ReturnError<addr_t>(OUT_OF_MEMORY, "Page table is out of space");
  };

  function<Result<Error, uint32_t>(uint32_t)> lift_size =
      [this, size](uint32_t _) { return ReturnOk(size); };

  return lift_size(size)
      .RightMap(bounds_check)
      .IfLeftElseRight(RightCompose(gc, RightCompose(lift_size, bounds_check)),
                       lift_size)
      .RightMap(allocate);
}

Result<Error, uint32_t> GarbageCollect() { return ReturnOk<uint32_t>(0); }

}  // namespace dlvm
