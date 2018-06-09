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

Result<Error, ValueType> MemoryManager::Insert(addr_t addr, ValueType value) {
  return this->Insert(addr, 0, value);
}

Result<Error, ValueType> MemoryManager::Insert(addr_t addr, uint32_t offset,
                                               ValueType value) {
  function<Result<Error, addr_t>(paddr_t)> bounds_check = BoundsCheck{offset};

  function<Result<Error, ValueType>(addr_t)> insert =
      [this, value](addr_t addr) -> Result<Error, ValueType> {
    Heap[addr] = value;
    return ReturnOk<>(value);
  };

  return TranslatePAddress(addr).RightMap(bounds_check).RightMap(insert);
}

Result<Error, ValueType> MemoryManager::Access(addr_t addr) {
  return this->Access(addr, 0);
}

Result<Error, ValueType> MemoryManager::Access(addr_t addr, uint32_t offset) {
  function<Result<Error, addr_t>(paddr_t)> bounds_check = BoundsCheck{offset};

  function<Result<Error, ValueType>(addr_t)> access =
      [this](addr_t addr) -> Result<Error, ValueType> {
    return this->Heap[addr].Unbox();
  };

  return TranslatePAddress(addr).RightMap(bounds_check).RightMap(access);
}

Result<Error, addr_t> MemoryManager::Malloc(uint32_t size) {
  function<Result<pair<Error, uint32_t>, uint32_t>(uint32_t)> bounds_check =
      [this](uint32_t size) {
        if (size + m_heap_alloc > heap_ptr) {
          return ReturnLeft <
                 Result<pair<Error, uint32_t>>(make_pair(
                     Error{OUT_OF_MEMORY, "Cannot allocate Heap"}, size));
        } else {
          return ReturnRight<Error, uint32_t>(size);
        }
      };

  function<Result<Error, uint32_t>(uint32_t)> gc =
      [this](pair<Error, uint32_t> p) {
        this->GarbageCollect();
        auto [e, size] = p;
        return ReturnOk<uint32_t>(size);
      };

  function<Result<Error, addr_t>(uint32_t)> allocate = [this](uint32_t size) {
    for (addr_t i = 0; i < m_pagetable_alloc; i++) {
      if (!PageTable[i].has_value()) {
        PageTable[i] = make_optional(make_pair(i, size));
      }
      return ReturnOk<addr_t>(i);
    }
    return ReturnError<addr_t>(OUT_OF_MEMORY, "Out of address space");
  };

  return ReturnOk<>(size)
      .RightMap(bounds_check)
      .template LeftMap<>(gc)
      .RightMap(allocate);
}

void GarbageCollect() { return; }

}  // namespace dlvm
