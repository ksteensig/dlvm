#include "interpreter.hpp"

using namespace dlvm;
using namespace std;

int main() {
    auto program = make_unique<vector<uint8_t>>(vector<uint8_t>{
            PUSH_INT,
            0, 0, 0, 0, 0, 0, 0, 10,
            CREATE_REFERENCE,
            HALT
    });

    auto settings = map<string, uint32_t>{};

    Interpreter interp{move(program), settings};

    interp.Execute();

    cout << "ValueType: " << sizeof(ValueType) << endl;
    cout << "VType: " << sizeof(VType) << endl;
    cout << "ReferenceType: " << sizeof(ReferenceType) << endl;
    cout << "RType: " << sizeof(RType) << endl;

    return 0;
}