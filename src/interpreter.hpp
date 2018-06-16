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

  Result<ValueType> PushUInt();
  Result<ValueType> PushInt();
  Result<ValueType> PushFloat();
  Result<ValueType> PushBool();
  Result<ValueType> Pop() { return m_memory->Pop(); }
  Result<ValueType> InsertArray();
  Result<ValueType> PushChar();
  Result<ValueType> Box();
  Result<ValueType> Unbox();
  Result<ValueType> CreateArray();
  Result<ValueType> AccessArray();
  Result<ValueType> InvokeNative();
  Result<ValueType> InvokeManaged();
  Result<ValueType> Return();
  Result<ValueType> Spawn();
  Result<ValueType> Join();

 public:
  /*
  Interpreter(unique_ptr<vector<uint8_t>> program,
             map<string, uint32_t> settings)
     : Program{move(program)}, Memory{MemoryManager{settings}} {}
  */
  void Execute();
};

function<Result<ValueType>(ValueType, ValueType)> ArithmeticAdd =
    ArithmeticFunctor{ADDOP};
function<Result<ValueType>(ValueType, ValueType)> ArithmeticSub =
    ArithmeticFunctor{SUBOP};
function<Result<ValueType>(ValueType, ValueType)> ArithmeticMul =
    ArithmeticFunctor{MULOP};
function<Result<ValueType>(ValueType, ValueType)> ArithmeticDiv =
    ArithmeticFunctor{DIVOP};

}  // namespace dlvm
