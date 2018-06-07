#include "mem.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

Result<paddr_t> HeapManager::TranslatePAddress(addr_t addr) {
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

Result<ValueType> HeapManager::Insert(addr_t addr, ValueType value) {
  return this->Insert(addr, 0, value);
}

Result<ValueType> HeapManager::Insert(addr_t addr, uint32_t offset,
                                      ValueType value) {
  function<Result<addr_t>(paddr_t)> bounds_check = BoundsCheck{offset};

  function<Result<ValueType>(addr_t)> insert =
      [this, value](addr_t addr) -> Result<ValueType> {
    this->Heap[addr] = value;
    return ReturnOk(value);
  };

  return TranslatePAddress(addr)
      .template Apply<>(bounds_check)
      .template Apply<>(insert);
}

Result<ValueType> HeapManager::Access(addr_t addr) {
  return this->Access(addr, 0);
}

Result<ValueType> HeapManager::Access(addr_t addr, uint32_t offset) {
  function<Result<addr_t>(paddr_t)> bounds_check = BoundsCheck{offset};

  function<Result<ValueType>(addr_t)> access =
      [this](addr_t addr) -> Result<ValueType> {
    return this->Heap[addr].Unbox();
  };

  return TranslatePAddress(addr)
      .template Apply<>(bounds_check)
      .template Apply<>(access);
}

Result<addr_t> HeapManager::Malloc(uint32_t size) {
  // GarbageCollect();
  function<Result<uint32_t>(uint32_t)> bounds_check =
      [this](uint32_t size) -> Result<uint32_t> {
    if (size + m_heap_alloc > heap_ptr) {
      return ReturnError<uint32_t>(OUT_OF_MEMORY,
                                   "Cannot allocate requested memory");
    } else {
      return ReturnOk(size);
    }
  };

  function<Result<addr_t>(uint32_t)> allocate =
      [this](uint32_t size) -> Result<addr_t> {
    for (addr_t i = 0; i < m_pagetable_alloc; i++) {
      if (!PageTable[i].has_value()) {
        PageTable[i] = make_optional(make_pair(i, size));
      }

      return ReturnOk(i);
    }

    return ReturnError<addr_t>(OUT_OF_MEMORY, "Out of address space");
  };

  return ReturnOk(size)
      .template Apply<>(bounds_check)
      .template Apply<>(allocate);
}

}  // namespace dlvm
