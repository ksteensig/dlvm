#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "function.hpp"
#include "opcode.hpp"
#include "type.hpp"
#include "vmm.hpp"

namespace dlvm {

using namespace std;
using namespace dlvm;

class Interpreter {
  MemoryManager Memory;

  unique_ptr<vector<uint8_t>> Program;
  uint64_t pc = 0;

  ValueType reg1, reg2, reg3, reg4;

  Result<ValueType> Pop();
  Result<ValueType> Push(ValueType obj);

  inline uint8_t Next();
  inline uint32_t NextQuad();
  inline uint64_t NextWord();

  Result<ValueType> PushUInt();
  Result<ValueType> PushInt();
  Result<ValueType> PushFloat();
  Result<ValueType> PushBool();
  Result<ValueType> PushChar();
  Result<ValueType> CreateReference();
  Result<ValueType> AccessReference();
  Result<ValueType> CreateArray();
  Result<ValueType> AccessArray();
  Result<ValueType> InsertArray();
  Result<ValueType> Call();
  Result<ValueType> Return();

 public:
  Interpreter(unique_ptr<vector<uint8_t>> program,
              map<string, uint32_t> settings)
      : Program{move(program)}, Memory{MemoryManager{settings}} {}

  void Execute();
};

}  // namespace dlvm
