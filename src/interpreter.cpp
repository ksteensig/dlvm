#include "interpreter.hpp"

namespace dlvm {

using namespace std;
using namespace dlvm;

void Interpreter::Execute() {
  auto op = Next();

  while (true) {
    switch (op) {
      case NOP:
        break;
      case POP:
        break;
      case PUSH_INT:
        break;
      case CREATE_REFERENCE:
        break;
      case PRINT:
        break;
      case HALT:
        return;
      default:
        return;
    }

    op = Next();
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
