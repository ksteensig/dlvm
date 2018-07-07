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
  DLVMModule module{m_name};

  module.m_imports = move(m_imports);
  module.m_native = move(m_native);
  module.m_managed = move(m_managed);
  module.m_program = move(m_program);

  return module;
}

}  // namespace dlvm