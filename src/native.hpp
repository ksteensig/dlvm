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
  Result<Error, ValueType> Push(ValueType value);
  Result<Error, ValueType> Pop();
  Result<Error, vector<ValueType>> GetArgs(uint8_t argc);
  Result<Error, ValueType> Return(Result<Error, ValueType> result);
  Result<Error, vector<ValueType>> Return(
      Result<Error, std::vector<ValueType>> result);

  Result<Error, ValueType> Insert(addr_t addr, uint32_t offset,
                                  ValueType value) {
    return m_memory->Insert(addr, offset, value);
  };
  Result<Error, ValueType> Insert(addr_t addr, ValueType value) {
    return Insert(addr, 0, value);
  };

  Result<Error, ValueType> Access(addr_t addr, uint32_t offset) {
    return m_memory->Access(addr, offset);
  };
  Result<Error, ValueType> Access(addr_t addr) { return Access(addr, 0); };

  Result<Error, addr_t> Malloc(uint32_t size);
};

// .so file
struct SharedObject {
  void *Library;
  string Name;
  vector<string> Handles;
};

struct DynamicLibraryLoader {
  vector<SharedObject> m_objects;
  Result<Error, optional<NativeFunc>> Load(string so_name, string handle);
  Result<Error, NativeFunc> Unload();
};

}  // namespace dlvm
