#pragma once

#include <cstdint>
#include <memory>
#include <variant>
#include <vector>

#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

typedef enum { NATIVE, MANAGED } FunctionType;

class NativeFunction {
  uint8_t m_argc;
  string m_handle;

  NativeFunction(uint8_t argc, string handle)
      : m_argc{argc}, m_handle{handle} {}

 public:
  Result<ReferenceType> Invoke();
};

class ManagedFunction {
  uint8_t m_argc;
  addr_t m_address;

 public:
  Result<ReferenceType> Invoke();
};

class Function {
  FunctionType m_ftype;
  variant<NativeFunction, ManagedFunction> m_function;

  Function(FunctionType ftype,
           variant<NativeFunction, ManagedFunction> function)
      : m_ftype{ftype}, m_function{function} {}

 public:
  Result<ReferenceType> Invoke() {
    switch (m_ftype) {
      case NATIVE:
        return get<NativeFunction>(m_function).Invoke();
      default:
        return LiftError<ReferenceType>(INVALID_ARGUMENT, "");
    }
  }
};

struct Library {
  string Name;
  vector<string> Handles;
};

class FunctionTable {
  Library m_libraries;
  vector<Function> m_functions;

  FunctionTable() {}

 public:
  void Load();
};

}  // namespace dlvm
