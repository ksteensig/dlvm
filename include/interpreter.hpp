#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "function.hpp"
#include "module.hpp"
#include "opcode.hpp"
#include "type.hpp"

namespace dlvm {

using std::shared_ptr;
using std::vector;
using namespace module;

#define EXCEPTION_HANDLE(name, action, error) \
  auto name##_res = action;                   \
  if (name##_res.IsError()) {                 \
    error;                                    \
  }                                           \
  auto name = name##_res.fromOk();

#define TYPE_CHECK(name, typ, error) \
  if (name.type != typ) {            \
    error;                           \
  }

class Interpreter {
  shared_ptr<uint8_t[]> m_program;
  uint32_t m_program_length;

  std::shared_ptr<DLVMModuleTable> module_table;
  std::shared_ptr<ConstantPool> constant_pool;

  uint32_t pc = 0;
  uint32_t fp = 0;
  uint32_t current_module = 0;

  shared_ptr<MemoryManager> m_memory;
  shared_ptr<NativeFunctionTable> m_native_table;
  shared_ptr<ManagedFunctionTable> m_managed_table;

  DLVMEnvironment* env;

  ValueType reg1, reg2, reg3, reg4;

  inline uint8_t Next();
  inline uint32_t NextQuad();
  inline uint64_t NextWord();

  Result<ValueType> Push(ValueType v) { return m_memory->Push(v); }
  Result<ValueType> Pop() { return m_memory->Pop(); }
  Result<ValueType> JumpOnTrue();
  Result<ValueType> InvokeManaged();
  Result<ValueType> InvokeNative();

  function<Result<ValueType>(ValueType)> push = [this](ValueType value) {
    return this->Push(value);
  };

 public:
  Interpreter(shared_ptr<uint8_t[]> program, shared_ptr<MemoryManager> memory,
              shared_ptr<NativeFunctionTable> native,
              shared_ptr<ManagedFunctionTable> managed)
      : m_program{program},
        m_memory{memory},
        m_native_table{native},
        m_managed_table{managed} {
    env = new DLVMEnvironment(m_memory);
  }

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
