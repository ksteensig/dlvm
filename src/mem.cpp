#include "mem.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

Result<paddr_t> MemoryManager::TranslatePAddress(addr_t addr) {
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

Result<ValueType> MemoryManager::Insert(addr_t addr, uint32_t offset,
                                        ValueType value) {
  auto insert = [this, value](addr_t addr) -> Result<ValueType> {
    Heap[addr] = value;
    return ReturnOk<ValueType>(value);
  };

  return TranslatePAddress(addr)
      .template MapOk<addr_t>(PageTableBoundsCheck{offset})
      .template MapOk<ValueType>(insert);
}

Result<ValueType> MemoryManager::Access(addr_t addr, uint32_t offset) {
  return TranslatePAddress(addr)
      .template MapOk<addr_t>(PageTableBoundsCheck{offset})
      .template MapOk<ValueType>(
          [this](addr_t addr) { return this->Heap[addr].Unbox(); });
}

Result<addr_t> MemoryManager::Malloc(uint32_t size) {
  auto bounds_check = [this](uint32_t size) {
    return HeapBoundsCheck{this->m_max_heap, this->heap_ptr}(size);
  };

  auto gc = [this](Error _) { return this->GarbageCollect(); };

  auto allocate = [this](uint32_t size) {
    for (addr_t i = 0; i < m_max_pagetable; i++) {
      if (!this->PageTable[i].has_value()) {
        this->PageTable[i] = make_optional(make_pair(i, size));
        return ReturnOk<addr_t>(i);
      }
    }
    return ReturnError<addr_t>(OUT_OF_MEMORY, "Page table is out of space");
  };

  auto lift_size = [this, size](uint32_t _) { return ReturnOk(size); };

  auto f = ComposeOk<addr_t>(lift_size, bounds_check);

  return lift_size(size)
      .template MapOk<uint32_t>(bounds_check)
      .template IfErrorElseOk<uint32_t>(ComposeOk<addr_t>(gc, f), lift_size)
      .template MapOk<addr_t>(allocate);
}

Result<ValueType> MemoryManager::Insert(ValueType addr, ValueType offset,
                                        ValueType value) {
  if (!(addr.type == PTR || addr.type == UINTEGER)) {
    return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }

  if (!(offset.type == UINTEGER)) {
    return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }

  return Insert(get<addr_t>(addr.Value), get<uint64_t>(offset.Value), value);
};

Result<ValueType> MemoryManager::Access(ValueType addr, ValueType offset) {
  if (!(addr.type == PTR || addr.type == UINTEGER)) {
    return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }

  if (!(offset.type == UINTEGER)) {
    return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }

  return Access(get<addr_t>(addr.Value), get<uint64_t>(offset.Value));
}

Result<ValueType> MemoryManager::Malloc(ValueType size) {
  if (!(size.type == UINTEGER)) {
    return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }

  auto r = Malloc(get<uint64_t>(size.Value));

  auto f = [](addr_t addr) { return ReturnOk(ValueType{PTR, addr}); };

  return r.template MapOk<ValueType>(f);
}

Result<uint32_t> GarbageCollect() { return ReturnOk<uint32_t>(0); }

}  // namespace dlvm
