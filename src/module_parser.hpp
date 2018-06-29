#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "function.hpp"
#include "opcode.hpp"

namespace dlvm {

using namespace std;
using namespace dlvm;

struct Module {
  shared_ptr<vector<string>> m_imports;
  shared_ptr<NativeFunctionTable> m_native;
  shared_ptr<ManagedFunctionTable> m_managed;
  shared_ptr<uint8_t[]> m_program;

}

class Linker {
 public:
  DLVMModule Parse();
}

}  // namespace dlvm
