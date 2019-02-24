#include "mem.hpp"

namespace dlvm {
Result<addr_t> MemoryManager::Malloc(uint32_t size) {
  if ((heap_ptr + size) > m_max_heap) {
    SuperBlockGC();
  }

  if ((heap_ptr + size) > m_max_heap) {
    BlockGC();
  }

  if ((heap_ptr + size) > m_max_heap) {
    return ReturnError<addr_t>(OUT_OF_MEMORY,
                               "Maximum heap size has been reached");
  }

  heap_ptr += size;

  return ReturnOk<addr_t>(size);
}

void MemoryManager::MarkHelper(addr_t i) {
  if (!Heap[i].Marked) {
    Heap[i].Marked = true;
    SuperBlockMarked[heap_ptr / page_size] = 1;
    switch (Heap[i].type) {
      case PTR:
        MarkHelper(get<addr_t>(Heap[i].Value));
        break;
      case ARRAY:
        for (addr_t i = 1; i < get<addr_t>(Heap[i].Value); i++) {
          MarkHelper(i + i);
        }
        break;
      default:
        break;
    }
  } else {
    return;
  }
}

void MemoryManager::Mark() {
  for (addr_t i = 0; i < stack_ptr; i++) {
    switch (Stack[i].type) {
      case PTR: {
        addr_t i = std::get<addr_t>(Stack[i].Value);
        MarkHelper(i);
        break;
      }
      default:
        break;
    }
  }
}

void MemoryManager::SuperBlockGC() {
  Mark();

  // uint32_t super_blocks = ((heap_ptr + page_size) / page_size) - 1;
  uint32_t curr_super_block = 0;
  uint32_t dead_blocks = 0;

  auto heap = Heap.get();

  for (addr_t i = 0; i < heap_ptr; i++) {
    // if the loop has just entered a new page
    if (!(heap_ptr % page_size)) {
      // if this new page does not have any marked ref values in it
      if (!SuperBlockMarked[curr_super_block]) {
        dead_blocks++;
        // skip all inside the page, except the last index,
        // the last index is skipped on next iteration as i++
        i += page_size - 1;
        continue;
      }
      curr_super_block = i / page_size;
    }

    if (Heap[i].Marked) {
      ForwardPointers.insert(std::make_pair(i, i - page_size * dead_blocks));
      Heap[i].Marked = false;
      switch (Heap[i].type) {
        case ARRAY: {
          addr_t v = get<addr_t>(Heap[i].Value);
          for (uint32_t j = 1; j < v; j++) {
            Heap[i + j].Marked = false;
          }
        }; break;
        default:
          break;
      }
    }
  }

  if (dead_blocks && !((heap_ptr + 1) % page_size)) {
    std::memcpy((void *)&heap[curr_super_block - dead_blocks],
                (void *)&heap[curr_super_block],
                page_size * sizeof(ReferenceType));
    SuperBlockMarked.at(curr_super_block) = 0;
  }

  for (addr_t i = 0; i < stack_ptr; i++) {
    switch (Stack[i].type) {
      case PTR: {
        auto p = ForwardPointers.find(i);
        Stack[i].Value = p->second;
      }; break;
      default:
        break;
    }
  }

  heap_ptr -= dead_blocks * page_size;

  for (addr_t i = 0; i < heap_ptr; i++) {
    switch (Heap[i].type) {
      case PTR: {
        auto p = ForwardPointers.find(i);
        Heap[i].Value = p->second;
      }; break;
      default:
        break;
    }
  }

  ForwardPointers.clear();
}

}  // namespace dlvm