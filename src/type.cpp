#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

ReferenceType ValueType::Box() { return ReferenceType{type, Value}; }

Result<ValueType> ReferenceType::Unbox() {
  switch (type) {
    case ARRAY:
    case STRING:
      return ReturnError<ValueType>(INVALID_ARGUMENT, "");
    default:
      return ReturnOk(ValueType{type, Value});
  }
}

}  // namespace dlvm
