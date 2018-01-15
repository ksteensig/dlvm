#include "interpreter.hpp"

using namespace dlvm;
using namespace std;

int main() {
    /*
    auto program = vector<uint8_t>{
            PUSH_INT,
            0, 0, 0, 0, 0, 0, 0, 10,
            CREATE_REFERENCE,
            HALT};

    VM vm{make_unique<vector<uint8_t>>(program), 4000, 16000};

    vm.Execute();
    */

    cout << "ValueType: " << sizeof(ValueType) << endl;
    cout << "VType: " << sizeof(VType) << endl;
    cout << "ReferenceType: " << sizeof(ReferenceType) << endl;
    cout << "RType: " << sizeof(RType) << endl;

    return 0;
}