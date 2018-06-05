//#include "interpreter.hpp"
#include "type.hpp"

using namespace dlvm;
using namespace std;
using namespace std::placeholders;

int main() {
  VType v1 = 1.1;
  VType v2 = (uint64_t)6;

  ValueType vt1{FLOAT, v1};
  ValueType vt2{UINTEGER, v2};

  Result<ValueType> r1 = LiftType(vt1);
  Result<ValueType> r2 = LiftType(vt2);

  Result<ValueType> r3 =
      MapResult2<ValueType, ValueType, ValueType>(Arithmetic2{ADDOP}, r1, r2);

  // cout << get<double>(get<ValueType>(r3.result).Value) << endl;

  /*
    auto program = make_unique<vector<uint8_t>>(vector<uint8_t>{
        PUSH_INT, 0, 0, 0, 0, 0, 0, 0, 10, CREATE_REFERENCE, HALT});

    auto settings = map<string, uint32_t>{};

    Interpreter interp{move(program), settings};

    interp.Execute();

    cout << "ValueType: " << sizeof(ValueType) << endl;
    cout << "VType: " << sizeof(VType) << endl;
    cout << "ReferenceType: " << sizeof(ReferenceType) << endl;
    cout << "RType: " << sizeof(RType) << endl;
  */
  return 0;
}
