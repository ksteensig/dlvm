#include "function.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

Result<ManagedFunction> ManagedFunctionTable::Get(ValueType v) {
  if (v.type == UINTEGER) {
    auto i = get<uint32_t>(v.Value);
    if (m_functions.size() > i) {
      return ReturnOk<>(m_functions[i]);
    }
  }

  return ReturnError<ManagedFunction>(OUT_OF_BOUNDS,
                                      "Calling non-existent function");
}

Result<vector<ValueType>> NativeFunction::Invoke(DLVMEnvironment *env) {
  func(env);
  return ReturnError<vector<ValueType>>(UNKNOWN, "");
}

Result<vector<ValueType>> NativeFunctionTable::Call(ValueType v,
                                                    DLVMEnvironment *env) {
  if (v.type == UINTEGER && m_functions.size() > get<uint32_t>(v.Value)) {
    return m_functions[get<uint32_t>(v.Value)].Invoke(env);
  } else {
    return ReturnError<vector<ValueType>>(OUT_OF_BOUNDS,
                                          "Calling non-existent function");
  }
}

Result<bool> NativeFunctionTable::Load(string so_name, string handle) {
  function<Result<bool>(optional<NativeFunc>)> add_func =
      [this](optional<NativeFunc> nfunc) {
        if (nfunc.has_value()) {
          this->m_functions.push_back(NativeFunc{nfunc.value()});
          return ReturnOk<>(true);
        } else {
          return ReturnOk<>(false);
        }
      };

  return m_library_loader.Load(so_name, handle).MapOk(add_func);
}

}  // namespace dlvm
