#include "native.hpp"

namespace dlvm {

Result<optional<NativeFunc>> DynamicLibraryLoader::Load(string so_name,
                                                        string handle) {
  SharedObject obj;

  obj.Name = so_name;
  obj.Library = dlopen(so_name.c_str(), RTLD_LAZY);
  obj.Handles.push_back(handle);

  if (!obj.Library) {
    return ReturnError<optional<NativeFunc>>(
        INVALID_ARGUMENT, "Shared object " + so_name + " could not be found");
  }

  void *handle_ref = dlsym(obj.Library, handle.c_str());

  if (!handle_ref) {
    return ReturnError<optional<NativeFunc>>(
        INVALID_ARGUMENT, "Function " + handle + " could not be found");
  }

  return ReturnOk(make_optional<NativeFunc>((NativeFunc)handle_ref));
}

Result<NativeFunc> DynamicLibraryLoader::Unload() {
  return ReturnError<NativeFunc>(UNKNOWN, "");
}

}  // namespace dlvm
