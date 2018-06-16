#include "interpreter.hpp"

namespace dlvm {

using namespace std;
using namespace dlvm;

ValueType _{};

void Interpreter::Execute() {
  function<Result<ValueType>(ValueType)> push = [this](ValueType value) {
    return this->m_memory->Push(value);
  };

  function<Result<ValueType>(ValueType)> pop = [this](ValueType _) {
    return this->m_memory->Pop();
  };

  function<Result<vector<ValueType>>(vector<ValueType>, ValueType)> push_back =
      [this](vector<ValueType> vec, ValueType v) {
        vec.push_back(v);
        return ReturnOk(vec);
      };

  while (true) {
    auto op = Next();
    switch (op) {
      case NOP:
        break;
      case POP:
        m_memory->Pop().OnError();
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
        Pop()
            .MapOk(static_cast<function<Result<ValueType>(ValueType)>>(
                [this](ValueType v) { return this->m_memory->Malloc(v); }))
            .MapOk(push)
            .OnError();
      case ACCESS_ARRAY:
        Pop()
            .AggregateOk(
                static_cast<function<Result<ValueType>(ValueType, ValueType)>>(
                    [this](ValueType v1, ValueType v2) {
                      return this->m_memory->Access(v1, v2);
                    }),
                Pop())
            .MapOk(push)
            .OnError();
      case INSERT_ARRAY:
        ReturnOk<vector<ValueType>>(vector<ValueType>{})
            .AggregateOk(push_back, Pop())
            .AggregateOk(push_back, Pop())
            .AggregateOk(push_back, Pop())
            .MapOk(static_cast<function<Result<ValueType>(vector<ValueType>)>>(
                [this](vector<ValueType> vec) {
                  return this->m_memory->Insert(vec.at(0), vec.at(1),
                                                vec.at(2));
                }))
            .MapOk(push)
            .OnError();
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
