#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "function.hpp"
#include "opcode.hpp"

namespace dlvm {

using namespace std;
using namespace dlvm;

struct _NativeFunction {
  string shared_obj_name;
  string function_handle_name;
};

struct _ManagedFunction {
  string module_name;  // module where the function comes from
  uint32_t addr;
  uint8_t argc;
};

struct DLVMModule {
  string name;
  unique_ptr<vector<string>> m_imports;
  unique_ptr<vector<_NativeFunction>> m_native;
  unique_ptr<vector<_ManagedFunction>> m_managed;
  unique_ptr<vector<uint8_t>> m_program;

  DLVMModule(string name) : name{name} {}
};

class DLVMModuleParser {
  istream& m_input;

  string m_name;
  unique_ptr<vector<string>> m_imports;
  unique_ptr<vector<_NativeFunction>> m_native;
  unique_ptr<vector<_ManagedFunction>> m_managed;
  unique_ptr<vector<uint8_t>> m_program;

  string ParseSingleImport();
  void ParseImports();
  void ParseNativeFunctions();
  void ParseManagedFunctions();
  _NativeFunction ParseSingleNativeFunction();
  _ManagedFunction ParseSingleManagedFunction();
  void ParseProgram();

 public:
  DLVMModuleParser(string name, istream input) : m_input{input}, m_name{name} {
    m_imports = make_unique<vector<string>>();
    m_native = make_unique<vector<_NativeFunction>>();
    m_managed = make_unique<vector<_ManagedFunction>>();
    m_program = make_unique<vector<uint8_t>>();
  }

  DLVMModule Parse();
};

class DLVMLoader {
  vector<string> m_paths{"./"};

  DLVMLoader(vector<string> paths, string main_module);

  void Load();
};

}  // namespace dlvm