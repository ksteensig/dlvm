#include <cstddef>
#include "interpreter.hpp"

using namespace dlvm;
using namespace std;
using namespace std::placeholders;

int main() {
  function<Result<Error, ValueType>(ValueType, ValueType)> ArithmeticAdd =
      ArithmeticFunctor{ADDOP};
  function<Result<Error, ValueType>(ValueType, ValueType)> ArithmeticSub =
      ArithmeticFunctor{SUBOP};
  function<Result<Error, ValueType>(ValueType, ValueType)> ArithmeticMul =
      ArithmeticFunctor{MULOP};
  function<Result<Error, ValueType>(ValueType, ValueType)> ArithmeticDiv =
      ArithmeticFunctor{DIVOP};

  VType v1 = 1.1;
  VType v2 = (uint64_t)6;

  ValueType vt1{FLOAT, v1};
  ValueType vt2{UINTEGER, v2};

  auto r1 = ReturnOk(vt1);
  auto r2 = ReturnOk(vt2);

  auto r3 = r1.RightZip(ArithmeticAdd, r2);

  NativeFunctionTable ft;

  // ft.Load("./libtest.so", "test_func");

  DLVMEnvironment env;

  ft.Call(0, &env);

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
