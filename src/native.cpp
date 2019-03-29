#include "native.hpp"

namespace dlvm {

Result<bool> NativeFunctionTable::Call(std::string so_name, std::string handle,
                                       DLVMEnvironment *env) {
  Result<bool> loaded = ReturnOk(true);

  auto so_search = m_functions.find(so_name);
  if (so_search == m_functions.end()) {
    loaded = m_library_loader.Load(so_name, handle);
  } else {
    auto fun_search = m_functions[so_name].find(handle);
    if (fun_search == m_functions[so_name].end()) {
      loaded = m_library_loader.Load(so_name, handle);
    }
  }

  if (loaded.IsError()) {
    return loaded;
  }

  auto f = m_functions[so_name][handle];
  f(env);

  return loaded;
}

Result<bool> DynamicLibraryLoader::Load(std::string so_name,
                                        std::string handle) {
  SharedObject obj;

  obj.Name = so_name;
  obj.Library = dlopen(so_name.c_str(), RTLD_LAZY);
  obj.Handles.push_back(handle);

  if (!obj.Library) {
    return ReturnError<bool>(
        INVALID_ARGUMENT, "Shared object " + so_name + " could not be found");
  }

  void *handle_ref = dlsym(obj.Library, handle.c_str());

  if (!handle_ref) {
    return ReturnError<bool>(INVALID_ARGUMENT,
                             "Function " + handle + " could not be found");
  }

  return ReturnOk(true);
}

Result<bool> DynamicLibraryLoader::Unload() {
  return ReturnError<bool>(UNKNOWN, "");
}

}  // namespace dlvm
