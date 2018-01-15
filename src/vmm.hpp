#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <algorithm>
#include <map>

#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

class MemoryManager {
    addr_t m_max_stack              = 4096;
    addr_t m_stack_alloc            = 4096;
    addr_t m_heap_alloc             = 8192;
    addr_t m_max_heap               = 8192;
    addr_t m_max_pagetable          = 8192;
    addr_t m_pagetable_alloc        = 8192;
    addr_t heap_ptr                 = 0;

    unique_ptr<ValueType[]>         Stack;
    unique_ptr<ReferenceType[]>     Heap;
    unique_ptr<vaddr_t[]>           PageTable;

    template<class T>
    void Grow(unique_ptr<T[]> array, uint32_t curr_size, uint32_t new_size);
    void Copy(addr_t src, addr_t dest, uint32_t size);
    void Reset(addr_t dest, uint32_t size);
    addr_t Move(addr_t dest);
    void ResetPageTable();
    void UpdatePageTable(addr_t old_addr, addr_t new_addr);
    
    inline bool                     OccupiedAddress(vaddr_t addr);
    inline vaddr_t                  TranslateAddress(addr_t addr);
    inline uint32_t                 Mark(addr_t addr);
    inline void                     Compact(uint32_t marked);
    void                            GarbageCollect();

    public:
    addr_t stack_ptr                = 0;
    addr_t frame_ptr                = 0;

    Result<ValueType>               Malloc(uint32_t size);
    Result<ValueType>               Push(ValueType value);
    Result<ValueType>               Pop();

    MemoryManager(map<string, uint32_t> settings)
        : Stack{make_unique<ValueType[]>(m_max_stack)}
        , Heap{make_unique<ReferenceType[]>(m_max_heap)}
        , PageTable{make_unique<vaddr_t[]>(m_max_pagetable)}
    { }
};

}