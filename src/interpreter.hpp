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

class Interpreter {
  // MemoryManager Memory;
  unique_ptr<uint8_t[]> m_program;
  uint32_t pc = 0;

  unique_ptr<MemoryManager> m_memory;

  ValueType reg1, reg2, reg3, reg4;

  inline uint8_t Next();
  inline uint32_t NextQuad();
  inline uint64_t NextWord();

  Result<Error, ValueType> PushUInt();
  Result<Error, ValueType> PushInt();
  Result<Error, ValueType> PushFloat();
  Result<Error, ValueType> PushBool();
  Result<Error, ValueType> InsertArray();
  Result<Error, ValueType> PushChar();
  Result<Error, ValueType> Box();
  Result<Error, ValueType> Unbox();
  Result<Error, ValueType> CreateArray();
  Result<Error, ValueType> AccessArray();
  Result<Error, ValueType> InvokeNative();
  Result<Error, ValueType> InvokeManaged();
  Result<Error, ValueType> Return();
  Result<Error, ValueType> Spawn();
  Result<Error, ValueType> Join();

 public:
  /*
  Interpreter(unique_ptr<vector<uint8_t>> program,
             map<string, uint32_t> settings)
     : Program{move(program)}, Memory{MemoryManager{settings}} {}
  */
  void Execute();
};

}  // namespace dlvm
