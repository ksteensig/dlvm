#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "function.hpp"
#include "interpreter.hpp"
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
  shared_ptr<vector<string>> m_imports;
  shared_ptr<vector<_NativeFunction>> m_native;
  shared_ptr<vector<_ManagedFunction>> m_managed;
  shared_ptr<vector<uint8_t>> m_program;

  DLVMModule(string name, shared_ptr<vector<string>> imports,
             shared_ptr<vector<_NativeFunction>> native,
             shared_ptr<vector<_ManagedFunction>> managed,
             shared_ptr<vector<uint8_t>> program)
      : name{name},
        m_imports{imports},
        m_native{native},
        m_managed{managed},
        m_program{program} {}
};

class DLVMModuleParser {
  istream& m_input;

  string m_name;
  shared_ptr<vector<string>> m_imports;
  shared_ptr<vector<_NativeFunction>> m_native;
  shared_ptr<vector<_ManagedFunction>> m_managed;
  shared_ptr<vector<uint8_t>> m_program;

  string ParseSingleImport();
  void ParseImports();
  void ParseNativeFunctions();
  void ParseManagedFunctions();
  _NativeFunction ParseSingleNativeFunction();
  _ManagedFunction ParseSingleManagedFunction();
  void ParseProgram();

 public:
  DLVMModuleParser(string name, istream& input) : m_input{input}, m_name{name} {
    m_imports = make_shared<vector<string>>();
    m_native = make_shared<vector<_NativeFunction>>();
    m_managed = make_shared<vector<_ManagedFunction>>();
    m_program = make_shared<vector<uint8_t>>();
  }

  DLVMModule Parse();
};

class DLVMFileLoader {
  vector<string> m_paths{"./"};
  string m_main_module;
  shared_ptr<vector<DLVMModule>> m_modules;

  void Load(string module_name);

 public:
  DLVMFileLoader(vector<string> paths, string main_module);

  shared_ptr<vector<DLVMModule>> Load();
};

class DLVMModuleLinker {
  shared_ptr<vector<DLVMModule>> m_modules;
  vector<NativeFunction> m_native;
  vector<ManagedFunction> m_managed;
  vector<uint8_t> m_program;

  uint32_t program = 0;
  uint32_t managed = 0;
  uint32_t native = 0;

  void LinkSingleManaged(DLVMModule& m);
  void LinkSingleNative(DLVMModule& m);

 public:
  DLVMModuleLinker(shared_ptr<vector<DLVMModule>> modules)
      : m_modules{modules} {}
  std::unique_ptr<Interpreter> Link();
};

}  // namespace dlvm
