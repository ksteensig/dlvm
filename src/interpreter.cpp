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
                POP_HANDLER();
                break;
            case PUSH_INT:
                PUSH_INT_HANDLER();
                break;
            case CREATE_REFERENCE:
                CREATE_REFERENCE_HANDLER();
                break;
            case PRINT:
                PRINT_HANDLER();
                break;
            case HALT:
                return;
            default:
                return;
        }

        op = Next();
    }
}

uint64_t Interpreter::NextEightBytes() {
    uint64_t value = 0;

    for (uint8_t i = 0; i < 8; i++) {
        value = (value << 8) | Next();
    }

    return value;
}

}