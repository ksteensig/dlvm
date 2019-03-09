#include "interpreter.hpp"

namespace dlvm {

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
        ReturnOk<>(ValueType{UINTEGER, (uint64_t)NextWord()})
            .MapOk(push)
            .OnError();
        break;
      case PUSH_INT:
        ReturnOk<>(ValueType{INTEGER, (int64_t)NextWord()})
            .MapOk(push)
            .OnError();
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
      case CREATE_ARRAY: {
        auto size =
            Pop()
                .AggregateOk(ArithmeticAdd,
                             ReturnOk<>(ValueType{UINTEGER, (uint64_t)1}))
                .fromOk();
        auto ptr = m_memory->Malloc(size).fromOk();
        auto arr = ptr;
        arr.type = ARRAY;
        m_memory->Push(ptr);
        m_memory->Insert(ptr, ValueType{UINTEGER, (uint64_t)0}, arr);
      }; break;
      case ACCESS_ARRAY: {
        auto addr = m_memory->Pop().fromOk();
        auto offset = m_memory->Pop().fromOk();

        auto arr = m_memory->Access(addr, offset).fromOk();

        if (arr.type != ARRAY) {
          ReturnError<bool>(TYPE_ERROR,
                            "Type error: Tried to access non-existing array")
              .OnError();
        } else if (std::get<addr_t>(arr.Value) <
                   std::get<uint64_t>(offset.Value)) {
          ReturnError<bool>(
              OUT_OF_BOUNDS,
              "Out of bounds: Tried to access outside of an array")
              .OnError();
        }
        offset = ReturnOk<>(offset)
                     .AggregateOk(ArithmeticAdd,
                                  ReturnOk<>(ValueType{UINTEGER, (uint64_t)1}))
                     .fromOk();

        auto value = m_memory->Access(addr, offset).fromOk();

        m_memory->Push(value);
      } break;
      case INSERT_ARRAY: {
        auto addr = m_memory->Pop().fromOk();
        auto offset = m_memory->Pop().fromOk();

        auto arr = m_memory->Access(addr, offset).fromOk();

        if (arr.type != ARRAY) {
          ReturnError<bool>(
              TYPE_ERROR, "Type error: Tried to insert into non-existing array")
              .OnError();
        } else if (std::get<addr_t>(arr.Value) <
                   std::get<uint64_t>(offset.Value)) {
          ReturnError<bool>(
              OUT_OF_BOUNDS,
              "Out of bounds: Tried to insert outside of an array")
              .OnError();
        }
        auto value = m_memory->Pop().fromOk();
        offset = ReturnOk<>(offset)
                     .AggregateOk(ArithmeticAdd,
                                  ReturnOk<>(ValueType{UINTEGER, (uint64_t)1}))
                     .fromOk();

        m_memory->Insert(addr, offset, value).OnError();
      } break;
      case JMPT:
        break;
      case JMP:
        this->pc = NextQuad();
        break;
      case INVOKE_MANAGED:
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
