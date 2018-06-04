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

inline uint8_t Interpreter::Next() { return Program->at(pc++); }

inline uint32_t Interpreter::NextQuad() {
  uint32_t value = 0;

  for (uint8_t i = 0; i < 4; i++) {
    value = (value << 8) | Next();
  }

  return value;
}

uint64_t Interpreter::NextWord() { return (NextQuad() << 0x20) | NextQuad(); }

}  // namespace dlvm
