#include "native.hpp"

namespace dlvm {

Result<std::optional<NativeFunc>> DynamicLibraryLoader::Load(
    std::string so_name, std::string handle) {
  SharedObject obj;

  obj.Name = so_name;
  obj.Library = dlopen(so_name.c_str(), RTLD_LAZY);
  obj.Handles.push_back(handle);

  if (!obj.Library) {
    return ReturnError<std::optional<NativeFunc>>(
        INVALID_ARGUMENT, "Shared object " + so_name + " could not be found");
  }

  void* handle_ref = dlsym(obj.Library, handle.c_str());

  if (!handle_ref) {
    return ReturnError<std::optional<NativeFunc>>(
        INVALID_ARGUMENT, "Function " + handle + " could not be found");
  }

  return ReturnOk(std::make_optional<NativeFunc>((NativeFunc)handle_ref));
}

Result<NativeFunc> DynamicLibraryLoader::Unload() {
  return ReturnError<NativeFunc>(UNKNOWN, "");
}

}  // namespace dlvm
