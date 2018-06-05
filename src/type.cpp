#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

ReferenceType ValueType::Box() { return ReferenceType{type, Value}; }

Result<ValueType> ReferenceType::Unbox() {
  switch (type) {
    case ARRAY:
    case STRING:
    case STRUCT:
      return LiftError<ValueType>(INVALID_ARGUMENT, "");
    default:
      return LiftType(ValueType{type, get<VType>(Value)});
  }
}

}  // namespace dlvm
