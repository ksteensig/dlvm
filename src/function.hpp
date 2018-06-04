#pragma once

#include <cstdint>
#include <memory>
#include <variant>
#include <vector>

#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

struct NativeFunction {
  uint8_t argc;
  RType (*NFunc)(uint8_t, unique_ptr<RType[]>);

  NativeFunction(uint8_t argc, unique_ptr<RType[]> args,
                 RType (*func)(uint8_t, unique_ptr<RType[]>))
      : argc{argc} {
    NFunc = func;
  }

  void Invoke();
};

struct ManagedFunction {
  uint8_t argc;
  addr_t Start;
};

typedef variant<NativeFunction, ManagedFunction> Function;

class FunctionTable {
 public:
  unique_ptr<vector<Function>> FunctionTable;
};

}  // namespace dlvm
