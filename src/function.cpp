#include "function.hpp"

#include <dlfcn.h>
#include <cstddef>
#include <iostream>

namespace dlvm {

using namespace dlvm;
using namespace std;

Result<Error, bool> FunctionTable::Load(string so_name, string handle) {
  NativeFunction nfunc;
  SharedObject obj;

  obj.Name = so_name;
  obj.Library = dlopen(so_name.c_str(), RTLD_LAZY);
  obj.Handles.push_back(handle);

  if (!obj.Library) {
    return ReturnError<bool>(
        INVALID_ARGUMENT, "Shared object " + so_name + " could not be found");
  }

  void* handle_ref = dlsym(obj.Library, handle.c_str());

  if (!handle_ref) {
    cout << "fug" << endl;
    return ReturnError<bool>(INVALID_ARGUMENT,
                             "Function " + handle + " could not be found");
  }

  NativeFunc func_ref = (NativeFunc)handle_ref;
  nfunc.func = func_ref;

  m_functions.push_back(Function{NATIVE, nfunc});

  return ReturnOk(true);
}

}  // namespace dlvm
