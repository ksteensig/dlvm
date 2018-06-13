#include "function.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

Result<Error, vector<ValueType>> NativeFunction::Invoke(DLVMEnvironment *env) {}

Result<Error, vector<ValueType>> NativeFunctionTable::Call(
    uint32_t index, DLVMEnvironment *env) {
  if (m_functions.size() > index) {
    return m_functions[index].Invoke(env);
  } else {
    return ReturnError<vector<ValueType>>(OUT_OF_BOUNDS,
                                          "Calling non-existent function");
  }
}  // namespace dlvm

Result<Error, bool> NativeFunctionTable::Load(string so_name, string handle) {
  function<Result<Error, bool>(optional<NativeFunc>)> add_func =
      [this](optional<NativeFunc> nfunc) {
        if (nfunc.has_value()) {
          this->m_functions.push_back(NativeFunc{nfunc.value()});
          return ReturnOk<>(true);
        } else {
          return ReturnOk<>(false);
        }
      };

  return m_library_loader.Load(so_name, handle).RightMap(add_func);
}

}  // namespace dlvm
