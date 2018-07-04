#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "function.hpp"
#include "opcode.hpp"

namespace dlvm {

using namespace std;
using namespace dlvm;

struct _Import {
  uint16_t module_name_length;
  vector<uin8_t> module_name;
};

struct _NativeFunction {
  uint16_t shared_obj_length;
  vector<uint8_t> shared_obj;
  uint16_t name_length;
  vector<uint8_t> name;
};

struct _ManagedFunction {
  uint16_t module_name_length;
  vector<uint8_t> module_name;
  uint32_t addr;
  uint8_t argc;
};

struct _Program {
  vector<uint8_t> program;
};

pair<_Import, istream> ParseImport(istream input) {
  _Import import;

  uint16_t length = (input.get() << 8) | input.get();
  for (uint16_t i = 0; i < length; i++) {
    // read utf8 char
  }

  import.module_name_length = length;
  return import;
}

pair<_NativeFunction, istream> ParseNativeFunction(istream input) {
  _NativeFunction nf;

  uint16_t shared_obj_length = (input.get() << 8) | input.get();
  for (uint16_t i = 0; i < shared_obj_length; i++) {
    // read utf8 char
  }

  uint16_t name_length = (input.get() << 8) | input.get();
  for (uint16_t i = 0; i < name_length; i++) {
    // read utf8 char
  }

  nf.shared_obj_length = shared_obj_length;
  nf.name_length = name_length;
  return nf;
}

pair<_ManagedFunction, istream> ParseManagedFunction(istream input) {
  _ManagedFunction mf;

  uint16_t module_name_length = (input.get() << 8) | input.get();
  for (uint16_t i = 0; i < module_name_length; i++) {
    // read utf8 char
  }

  uint32_t addr = (input.get() << 24) | (input.get() << 16) |
                  (input.get() << 8) | input.get();

  uint8_t argc = input.get();

  mf.module_name_length = module_name_length;
  mf.addr = addr;
  mf.argc = argc;

  return mf;
}

pair<_Program, istream> ParseProgram(istream input) {
  _Program p;

  while (!input.eof()) {
    p.program.push_back(input.get());
  }

  return p;
}

struct Module {
  uint64_t magic_word;
  shared_ptr<vector<_Import>> m_imports;
  shared_ptr<vector<_NativeFunction>> m_native;
  shared_ptr<vector<_ManagedFunction>> m_managed;
  shared_ptr<_Program> m_program;
};

struct Loader {
  vector<string> m_paths{"./"};
  DLVMModule Parse();
};

class Linker {
 public:
};

}  // namespace dlvm
