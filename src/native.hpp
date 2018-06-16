#pragma once

#include <cstdint>
#include <memory>
#include <variant>
#include <vector>

#include <dlfcn.h>
#include <cstddef>

#include "mem.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

class DLVMEnvironment {
  shared_ptr<MemoryManager> m_memory;

 public:
  Result<ValueType> Push(ValueType value);
  Result<ValueType> Pop();
  Result<vector<ValueType>> GetArgs(uint8_t argc);
  Result<ValueType> Return(Result<ValueType> result);
  Result<vector<ValueType>> Return(Result<std::vector<ValueType>> result);

  Result<ValueType> Insert(addr_t addr, uint32_t offset, ValueType value) {
    return m_memory->Insert(addr, offset, value);
  };
  Result<ValueType> Insert(addr_t addr, ValueType value) {
    return Insert(addr, 0, value);
  };

  Result<ValueType> Access(addr_t addr, uint32_t offset) {
    return m_memory->Access(addr, offset);
  };
  Result<ValueType> Access(addr_t addr) { return Access(addr, 0); };

  Result<addr_t> Malloc(uint32_t size);
};

// .so file
struct SharedObject {
  void *Library;
  string Name;
  vector<string> Handles;
};

struct DynamicLibraryLoader {
  vector<SharedObject> m_objects;
  Result<optional<NativeFunc>> Load(string so_name, string handle);
  Result<NativeFunc> Unload();
};

}  // namespace dlvm
