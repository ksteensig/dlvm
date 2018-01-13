#include "vm.hpp"

namespace dlvm {

using namespace std;
using namespace dlvm;

void VM::Execute() {
    
    auto op = Next();
    
    while (true) {
        switch (op) {
            case NOP:
                break;
            case POP:
                POP_HANDLER();
                break;
            case PUSH_INT:
                cout << "push" << endl;
                PUSH_INT_HANDLER();
                break;
            case PRINT:
                cout << "print" << endl;
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

}