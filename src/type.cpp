#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

ReferenceType ValueType::Box() { return ReferenceType{type, Value}; }

Result<ValueType> ReferenceType::Unbox() {
  return ReturnOk<ValueType>(ValueType{type, Value});
}

}  // namespace dlvm
