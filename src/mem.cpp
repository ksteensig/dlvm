#include "mem.hpp"

namespace dlvm {

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
  function<Result<ValueType>(addr_t)> insert =
      [this, value](addr_t addr) -> Result<ValueType> {
    Heap[addr] = value;
    return ReturnOk<ValueType>(value);
  };

  return TranslatePAddress(addr)
      .MapOk(static_cast<function<Result<addr_t>(paddr_t)>>(
          PageTableBoundsCheck{offset}))
      .MapOk(insert);
}

Result<ValueType> MemoryManager::Access(addr_t addr, uint32_t offset) {
  return TranslatePAddress(addr)
      .MapOk(static_cast<function<Result<addr_t>(paddr_t)>>(
          PageTableBoundsCheck{offset}))
      .MapOk(static_cast<function<Result<ValueType>(addr_t)>>(
          [this](addr_t addr) { return this->Heap[addr].Unbox(); }));
}

Result<addr_t> MemoryManager::Malloc(uint32_t size) {
  function<Result<uint32_t>(uint32_t)> bounds_check = [this](uint32_t size) {
    return HeapBoundsCheck{this->m_max_heap, this->heap_ptr}(size);
  };

  function<Result<uint32_t>(Error)> gc = [this](Error _) {
    return this->GarbageCollect();
  };

  function<Result<addr_t>(uint32_t)> allocate = [this](uint32_t size) {
    for (addr_t i = 0; i < m_max_pagetable; i++) {
      if (!this->PageTable[i].has_value()) {
        this->PageTable[i] = std::make_optional(std::make_pair(i, size));
        return ReturnOk<addr_t>(i);
      }
    }
    return ReturnError<addr_t>(OUT_OF_MEMORY, "Page table is out of space");
  };

  function<Result<uint32_t>(uint32_t)> lift_size = [this, size](uint32_t _) {
    return ReturnOk(size);
  };

  return lift_size(size)
      .MapOk(bounds_check)
      .IfErrorElseOk(ComposeOk(gc, ComposeOk(lift_size, bounds_check)),
                     lift_size)
      .MapOk(allocate);
}

Result<ValueType> MemoryManager::Insert(ValueType addr, ValueType offset,
                                        ValueType value) {
  if (!(addr.type == PTR || addr.type == UINTEGER)) {
    return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }

  if (!(offset.type == UINTEGER)) {
    return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }

  return Insert(std::get<addr_t>(addr.Value), std::get<uint64_t>(offset.Value),
                value);
};

Result<ValueType> MemoryManager::Access(ValueType addr, ValueType offset) {
  if (!(addr.type == PTR || addr.type == UINTEGER)) {
    return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }

  if (!(offset.type == UINTEGER)) {
    return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }

  return Access(std::get<addr_t>(addr.Value), std::get<uint64_t>(offset.Value));
}

Result<ValueType> MemoryManager::Malloc(ValueType size) {
  if (!(size.type == UINTEGER)) {
    return ReturnError<ValueType>(INVALID_ARGUMENT, "");
  }

  auto r = Malloc(std::get<uint64_t>(size.Value));

  function<Result<ValueType>(addr_t)> f = [](addr_t addr) {
    return ReturnOk(ValueType{PTR, addr});
  };

  return r.MapOk(f);
}

Result<uint32_t> GarbageCollect() { return ReturnOk<uint32_t>(0); }

}  // namespace dlvm
