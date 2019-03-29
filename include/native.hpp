#pragma once

#include <dlfcn.h>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

#include "mem.hpp"
#include "type.hpp"

namespace dlvm {

using std::vector;

// .so file
struct SharedObject {
  void *Library;
  std::string Name;
  vector<std::string> Handles;
};

struct DynamicLibraryLoader {
  vector<SharedObject> m_objects;
  Result<bool> Load(std::string so_name, std::string handle);
  Result<bool> Unload();
};

class DLVMEnvironment {
  shared_ptr<MemoryManager> m_memory;

 public:
  DLVMEnvironment(shared_ptr<MemoryManager> memory_manager)
      : m_memory{memory_manager} {}

  Result<ValueType> Push(ValueType value);
  Result<ValueType> Pop();
  Result<vector<ValueType>> GetArgs(uint8_t argc);
  Result<ValueType> Return(Result<ValueType> result);
  Result<vector<ValueType>> Return(Result<std::vector<ValueType>> result);

  Result<ValueType> Insert(addr_t addr, uint32_t offset, ValueType value){
      // return m_memory->Insert(addr, offset, value);
  };
  Result<ValueType> Insert(addr_t addr, ValueType value){
      // return Insert(addr, 0, value);
  };

  Result<ValueType> Access(addr_t addr, uint32_t offset){
      // return m_memory->Access(addr, offset);
  };
  Result<ValueType> Access(addr_t addr) { return Access(addr, 0); };

  Result<addr_t> Malloc(uint32_t size);
};

class NativeFunctionTable {
 private:
  DynamicLibraryLoader m_library_loader;
  std::map<std::string, std::map<std::string, NativeFunc>> m_functions;

  // false if function already has been loaded
  Result<bool> Load(std::string so_name, std::string handle);
  Result<bool> Unload();

 public:
  NativeFunctionTable() {}
  Result<bool> Call(std::string so_name, std::string handle,
                    DLVMEnvironment *env);
};
}  // namespace dlvm
