#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <map>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "type.hpp"

#if defined(__unix__) || defined(__CYGWIN__) || \
    (defined(__APPLE__) && defined(__MACH__))
#include "posix.hpp"
#endif

#if (defined _WIN32 || defined __WIN32__) && !defined __CYGWIN__
#include "windows.hpp"
#endif

namespace dlvm {

using std::function;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

template <typename T>
class Memory {
 private:
  uint32_t pages;
  T *memory;

 public:
  T &operator[](const addr_t index) { return memory[index]; }

  T *get() { return memory; }

  Memory(uint64_t size) {
    // round up to next integer factor page size
    pages = (size + get_page_size()) / get_page_size() - 1;
    memory = (T *)allocate_memory(pages);
  }

  ~Memory() { deallocate_memory(memory, pages); }
};

class MemoryManager {
  addr_t m_max_heap = 8192;
  addr_t m_max_stack = 4096;

  addr_t heap_ptr = 0;

  std::map<addr_t, addr_t> ForwardPointers;
  std::vector<uint8_t> SuperBlockMarked;
  Memory<ReferenceType> Heap;
  Memory<ValueType> Stack;

  uint32_t page_size;

  void MarkHelper(addr_t i);
  void Mark();
  void UnMark();
  void Sweep();
  void SuperBlockGC();
  void BlockGC();

 public:
  MemoryManager(addr_t max_heap_size, addr_t max_stack_size)
      : m_max_heap{max_heap_size},
        m_max_stack{max_stack_size},
        Heap{Memory<ReferenceType>{max_heap_size}},
        Stack{Memory<ValueType>{max_stack_size}} {
    page_size = get_page_size() / sizeof(ReferenceType);
    SuperBlockMarked.reserve(m_max_heap / page_size);
  }

  addr_t stack_ptr = 0;

  Result<ValueType> Insert(ValueType addr, ValueType offset, ValueType value);

  Result<ValueType> Insert(addr_t addr, uint32_t offset, ValueType value);

  Result<ValueType> Access(ValueType addr, ValueType offset);

  Result<ValueType> Access(addr_t addr, uint32_t offset);

  Result<ValueType> Malloc(ValueType size);

  Result<addr_t> Malloc(uint32_t size);

  Result<ValueType> Push(ValueType value);
  Result<ValueType> Pop();
};

}  // namespace dlvm
