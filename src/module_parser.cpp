#include "module_parser.hpp"

namespace dlvm {

void DLVMModuleParser::ParseImports() {
  uint16_t count = (m_input.get() << 8) | m_input.get();

  for (uint16_t i = 0; i < count; i++) {
    m_imports->push_back(ParseSingleImport());
  }
}

string DLVMModuleParser::ParseSingleImport() {
  uint16_t length = (m_input.get() << 8) | m_input.get();
  auto name = vector<uint8_t>{};

  for (uint16_t i = 0; i < length; i++) {
    name.push_back(m_input.get());
  }

  return std::string(name.begin(), name.end());
}

_NativeFunction DLVMModuleParser::ParseSingleNativeFunction() {
  _NativeFunction nf;

  uint16_t shared_obj_length = (m_input.get() << 8) | m_input.get();
  auto shared_obj_name = vector<uint8_t>{};

  for (uint16_t i = 0; i < shared_obj_length; i++) {
    shared_obj_name.push_back(m_input.get());
  }

  uint16_t name_length = (m_input.get() << 8) | m_input.get();
  auto function_name = vector<uint8_t>{};

  for (uint16_t i = 0; i < name_length; i++) {
    function_name.push_back(m_input.get());
  }

  nf.shared_obj_name =
      std::string(shared_obj_name.begin(), shared_obj_name.end());
  nf.function_handle_name =
      std::string(function_name.begin(), function_name.end());

  return nf;
}

void DLVMModuleParser::ParseManagedFunctions() {
  uint32_t count = (m_input.get() << 24) | (m_input.get() << 16) |
                   (m_input.get() << 8) | m_input.get();

  for (uint32_t i = 0; i < count; i++) {
    m_managed->push_back(ParseSingleManagedFunction());
  }
}

_ManagedFunction DLVMModuleParser::ParseSingleManagedFunction() {
  _ManagedFunction mf;

  uint16_t module_name_length = (m_input.get() << 8) | m_input.get();
  auto module_name = vector<uint8_t>{};

  for (uint16_t i = 0; i < module_name_length; i++) {
    module_name.push_back(m_input.get());
  }

  uint32_t addr = (m_input.get() << 24) | (m_input.get() << 16) |
                  (m_input.get() << 8) | m_input.get();

  uint8_t argc = m_input.get();

  mf.module_name = std::string(module_name.begin(), module_name.end());
  mf.addr = addr;
  mf.argc = argc;

  return mf;
}

void DLVMModuleParser::ParseProgram() {
  while (!m_input.eof()) {
    m_program->push_back(m_input.get());
  }
}

DLVMModule DLVMModuleParser::Parse() {
  ParseImports();
  ParseNativeFunctions();
  ParseManagedFunctions();
  ParseProgram();

  DLVMModule module{module.name, m_imports, m_native, m_managed, m_program};

  return module;
}

DLVMFileLoader::DLVMFileLoader(vector<string> paths, string main_module)
    : m_paths{paths}, m_main_module{main_module} {
  m_modules = make_shared<vector<DLVMModule>>();
}

shared_ptr<vector<DLVMModule>> DLVMFileLoader::Load() {
  Load(m_main_module);
  return m_modules;
}

void DLVMFileLoader::Load(string module_name) {
  fstream module_stream;
  module_stream.open(module_name);

  DLVMModuleParser module_parser{module_name, module_stream};
  auto module = module_parser.Parse();
  module_stream.close();
  m_modules->push_back(module);

  // traverse the import tree by depth first.
  for (uint16_t i = 0; i < module.m_imports->size(); i++) {
    if (!any_of(module.m_imports->begin(), module.m_imports->end(),
                [module_name](string m) { return m == module_name; })) {
      Load(module.m_imports->at(i));
    }
  }
}

// add offset for modules using the functions from DLVMModule m
void DLVMModuleLinker::LinkSingleManaged(DLVMModule &m) {
  if (m.m_managed->empty()) {
    return;
  }

  map<uint32_t, uint32_t> managed_location;

  uint32_t i = 0;

  for (auto &f : *(m.m_managed)) {
    if (f.module_name == m.name) {
      f.addr += program;
      managed_location[f.addr] = i++;
      m_managed.push_back(ManagedFunction{f.argc, f.addr});
    }
  }

  for (auto &mo : *m_modules) {
    for (auto &f : *(mo.m_managed)) {
      if (f.module_name == m.name) {
        auto addr = managed + managed_location.at(f.addr);
        // insert values of managed into the 4 bytes after the function addr
        std::copy_n(&mo.m_program->at(f.addr), 0x04, &(addr));
      }
    }
  }

  managed += i;
}

void DLVMModuleLinker::LinkSingleNative(DLVMModule &m) {
  if (m.m_managed->empty()) {
    return;
  }
}

std::unique_ptr<Interpreter> DLVMModuleLinker::Link() {
  for (auto &m : *(m_modules)) {
    LinkSingleManaged(m);
    LinkSingleNative(m);
    program += m.m_program->size();
  }
}

}  // namespace dlvm
