#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

ReferenceType ValueType::Box() { return ReferenceType{type, Value}; }

Result<Error, ValueType> ReferenceType::Unbox() {
  switch (type) {
    default:
      return ReturnOk(ValueType{type, Value});
  }
}

}  // namespace dlvm
