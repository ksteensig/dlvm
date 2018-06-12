#pragma once

#include <dlfcn.h>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

typedef enum { NATIVE, MANAGED } FunctionType;

struct NativeFunction {
  NativeFunc func;
  Result<Error, ReferenceType> Invoke();
};

struct ManagedFunction {
  uint8_t m_argc;
  addr_t m_address;
  Result<Error, ReferenceType> Invoke();
};

struct Function {
  FunctionType m_ftype;
  variant<NativeFunction, ManagedFunction> m_function;

  // public:
  Function(FunctionType ftype,
           variant<NativeFunction, ManagedFunction> function)
      : m_ftype{ftype}, m_function{function} {}
  /*
Result<Error, ReferenceType> Invoke() {
 switch (m_ftype) {
   case NATIVE:
     return ReturnOk(get<NativeFunction>(m_function).Invoke());
   default:
     return ReturnError<ReferenceType>(INVALID_ARGUMENT, "");
 }
}
*/
};

// .so file
struct SharedObject {
  void *Library;
  string Name;
  vector<string> Handles;
};

struct FunctionTable {
  vector<SharedObject> m_objects;
  vector<Function> m_functions;

  FunctionTable() {}

  // false if function already has been loaded
  Result<Error, bool> Load(string so_name, string handle);
};

}  // namespace dlvm
