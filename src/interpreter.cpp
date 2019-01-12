#include "interpreter.hpp"

namespace dlvm {

Result<ValueType> Interpreter::CreateArray() {
  return Pop()
      .MapOk(static_cast<function<Result<ValueType>(ValueType)>>(
          [this](ValueType v) { return this->m_memory->Malloc(v); }))
      .MapOk(push);
}

Result<ValueType> Interpreter::InsertArray() {
  return ReturnOk<vector<ValueType>>(vector<ValueType>{})
      .AggregateOk(push_back, Pop())
      .AggregateOk(push_back, Pop())
      .AggregateOk(push_back, Pop())
      .MapOk(static_cast<function<Result<ValueType>(std::vector<ValueType>)>>(
          [this](vector<ValueType> vec) {
            return this->m_memory->Insert(vec.at(0), vec.at(1), vec.at(2));
          }))
      .MapOk(push);
}

Result<ValueType> Interpreter::AccessArray() {
  return Pop()
      .AggregateOk(
          static_cast<function<Result<ValueType>(ValueType, ValueType)>>(
              [this](ValueType v1, ValueType v2) {
                return this->m_memory->Access(v1, v2);
              }),
          Pop())
      .MapOk(push);
}

Result<ValueType> Interpreter::JumpOnTrue() {
  uint32_t pc_ = this->NextQuad();
  return Pop()
      .MapOk(static_cast<function<Result<ValueType>(ValueType)>>(
          [this](ValueType v) {
            if (v.type == BOOL && std::get<bool>(v.Value)) {
              return ReturnOk(v);
            } else {
              return ReturnError<ValueType>(UNKNOWN, "");
            }
          }))
      .MapOk(static_cast<function<Result<ValueType>(ValueType)>>(
          [pc_, this](ValueType v) {
            this->pc = pc_;
            return ReturnOk(v);
          }));
}

Result<ValueType> Interpreter::InvokeManaged() {
  function<Result<ManagedFunction>(ValueType)> get_func = [this](ValueType v) {
    return this->m_managed_table->Get(v);
  };

  function<Result<ValueType>(ManagedFunction)> call =
      [this](ManagedFunction m) {
        this->Push(ValueType{UINTEGER, static_cast<uint32_t>(m.m_argc)});
        this->Push(ValueType{UINTEGER, this->fp});
        this->Push(ValueType{UINTEGER, this->pc});
        this->fp = this->m_memory->stack_ptr;
        this->pc = m.m_address;
        return ReturnOk(ValueType{});
      };

  return Pop().MapOk(get_func).MapOk(call);
}

/*
Result<ValueType> Interpreter::InvokeNative() {
  Result<vector<ValueType>> call = [this](ValueType v) {
    return this->m_native_table->Call(v, this->env);
  };

  auto v = Pop().MapOk(call);
}
*/

void Interpreter::Execute() {
  while (true) {
    auto op = Next();
    switch (op) {
      case NOP:
        break;
      case POP:
        Pop().OnError();
        break;
      case PUSH_UINT:
        ReturnOk<>(ValueType{FLOAT, (uint64_t)NextWord()})
            .MapOk(push)
            .OnError();
        break;
      case PUSH_INT:
        ReturnOk<>(ValueType{FLOAT, (int64_t)NextWord()}).MapOk(push).OnError();
        break;
      case PUSH_FLOAT:
        ReturnOk<>(ValueType{FLOAT, (double)NextWord()}).MapOk(push).OnError();
        break;
      case PUSH_BOOL:
        ReturnOk<>(ValueType{BOOL, (bool)Next()}).MapOk(push).OnError();
        break;
      case ADD:
        Pop().AggregateOk(ArithmeticAdd, Pop()).MapOk(push).OnError();
        break;
      case SUB:
        Pop().AggregateOk(ArithmeticSub, Pop()).MapOk(push).OnError();
        break;
      case MUL:
        Pop().AggregateOk(ArithmeticMul, Pop()).MapOk(push).OnError();
        break;
      case DIV:
        Pop().AggregateOk(ArithmeticDiv, Pop()).MapOk(push).OnError();
        break;
      case AND:
      case OR:
      case XOR:
      case NOT:
      case LT:
      case LE:
      case EQ:
      case GE:
      case GT:
      case CREATE_ARRAY:
        CreateArray().OnError();
        break;
      case ACCESS_ARRAY:
        AccessArray().OnError();
        break;
      case INSERT_ARRAY:
        InsertArray().OnError();
        break;
      case JMPT:
        JumpOnTrue().OnError();
        break;
      case JMP:
        this->pc = NextQuad();
        break;
      case INVOKE_MANAGED:
        this->InvokeManaged().OnError();
        break;
      case INVOKE_NATIVE:
        // this->InvokeNative().OnError();
        break;
      case HALT:
      default:
        return;
    }
  }
}

inline uint8_t Interpreter::Next() { return m_program[pc++]; }

inline uint32_t Interpreter::NextQuad() {
  return (Next() << 0x18) | (Next() << 0x10) | (Next() << 0x08) | Next();
}

inline uint64_t Interpreter::NextWord() {
  return (static_cast<uint64_t>(NextQuad()) << 0x20) | NextQuad();
}

}  // namespace dlvm
