#pragma once

#include <dlfcn.h>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

#include "native.hpp"
#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

typedef enum { NATIVE, MANAGED } FunctionType;

struct NativeFunction {
  NativeFunc func;

  NativeFunction(NativeFunc func) : func{func} {}
  Result<Error, vector<ValueType>> Invoke(DLVMEnvironment *env);
};

struct ManagedFunction {
  uint8_t m_argc;
  addr_t m_address;
};

struct ManagedFunctionTable {
  vector<ManagedFunction> m_functions;
};

struct NativeFunctionTable {
  DynamicLibraryLoader m_library_loader;
  vector<NativeFunction> m_functions;

  NativeFunctionTable() {}

 public:
  // false if function already has been loaded
  Result<Error, bool> Load(string so_name, string handle);
  Result<Error, bool> Unload();
  Result<Error, vector<ValueType>> Call(uint32_t index, DLVMEnvironment *env);
};

}  // namespace dlvm
