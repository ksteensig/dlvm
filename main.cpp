#include "vm.hpp"

using namespace dlvm;
using namespace std;

int main() {
    auto program = vector<uint8_t>{
            PUSH_INT,
            0, 0, 0, 0, 0, 0, 0, 10,
            PRINT,
            HALT};

    VM vm{make_unique<vector<uint8_t>>(program)};

    vm.Execute();

    return 0;
}