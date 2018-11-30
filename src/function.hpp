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
  Result<vector<ValueType>> Invoke(DLVMEnvironment *env);
};

// name of function
// address of function in function table
// amount of arguments
struct ManagedFunction {
  string name;
  addr_t m_address;
  uint8_t m_argc;
};

struct ManagedFunctionTable {
  vector<ManagedFunction> m_functions;
  Result<ManagedFunction> Get(ValueType);
};

struct NativeFunctionTable {
  DynamicLibraryLoader m_library_loader;
  vector<NativeFunction> m_functions;

  NativeFunctionTable() {}

 public:
  // false if function already has been loaded
  Result<bool> Load(string so_name, string handle);
  Result<bool> Unload();
  Result<vector<ValueType>> Call(ValueType v, DLVMEnvironment *env);
};

}  // namespace dlvm
