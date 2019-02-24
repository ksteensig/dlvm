#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "function.hpp"
#include "interpreter.hpp"
#include "opcode.hpp"

namespace dlvm {

namespace module {

#define MAGIC_NUMBER 0xBADC0C0A

using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

enum class ConstantPoolEntryTypeTag {
  MANAGED_FUNCTION_REF,
  NATIVE_FUNCTION_REF,
  STRING,
  UINT32,
  UINT64,
  INT64,
  DOUBLE,
  BOOL
};

// (uint32_t, uint32_t) = first uint points to a module name, second uint points
// to a function name, these can then be used to look up the function address in
// the module table
using constant_pool_entry_t =
    std::variant<std::pair<uint32_t, uint32_t>, std::string, uint32_t, uint64_t,
                 int64_t, double, uint8_t>;

struct ConstantPoolEntry {
  ConstantPoolEntryTypeTag type;
  constant_pool_entry_t data;
};

class ConstantPool {
 public:
  std::vector<ConstantPoolEntry> m_entries;
  ConstantPool() {}

  void addEntry(ConstantPoolEntry entry) { m_entries.push_back(entry); }
  void addConstantPool(std::shared_ptr<ConstantPool> pool) {
    m_entries.insert(m_entries.end(), pool->m_entries.begin(),
                     pool->m_entries.end());
  }
};

struct DLVMParsedModule {
  uint32_t magic_number = 0xBADC0C0A;
  std::string module_name;

  uint32_t constant_pool_length = 0;
  uint32_t function_table_length = 0;
  uint32_t byte_code_length = 0;

  std::shared_ptr<ConstantPool> constant_pool;

  std::shared_ptr<std::vector<std::pair<std::string, uint32_t>>> function_table;

  std::shared_ptr<uint8_t[]> byte_code;
};

class DLVMModuleStreamParser {
 private:
  std::istream &m_input;
  shared_ptr<uint8_t[]> buffer;
  uint32_t buffer_len = 0;
  uint32_t index = 0;

  shared_ptr<ConstantPool> constant_pool;
  shared_ptr<std::vector<std::pair<std::string, uint32_t>>> function_table;

  uint8_t getByte() { return buffer[index++]; }

  int64_t parseInt64() {
    uint64_t number;
    std::memcpy(&number, buffer.get(), sizeof(int64_t) / sizeof(uint8_t));
    index += sizeof(int64_t);

    return number;
  }

  uint32_t parseUInt32() {
    uint32_t number;
    std::memcpy(&number, buffer.get(), sizeof(uint32_t) / sizeof(uint8_t));
    index += sizeof(uint32_t);

    return number;
  }

  uint64_t parseUInt64() {
    uint64_t number;
    std::memcpy(&number, buffer.get(), sizeof(uint64_t) / sizeof(uint8_t));
    index += sizeof(uint64_t);

    return number;
  }

  uint8_t parseBool() { return getByte(); }

  double parseDouble() {
    double number;
    std::memcpy(&number, buffer.get(), sizeof(double) / sizeof(uint8_t));
    index += sizeof(double);

    return number;
  }

  std::string parseString() {
    std::vector<uint8_t> str;
    while (buffer[index++] != '0') {
      str.push_back(buffer[index - 1]);
    }

    return string(str.begin(), str.end());
  }

  std::pair<uint32_t, uint32_t> parseManagedFunctionRef() {
    uint32_t module_name_index = parseUInt32();
    uint32_t function_name_index = parseUInt32();

    return std::make_pair(module_name_index, function_name_index);
  }

  std::pair<uint32_t, uint32_t> parseNativeFunctionRef() {
    uint32_t shared_obj_filename_index = parseUInt32();
    uint32_t function_name_index = parseUInt32();

    return std::make_pair(shared_obj_filename_index, function_name_index);
  }

  std::pair<std::string, uint32_t> parseFunctionTableElement() {
    std::string function_name = parseString();
    uint32_t function_code_index = parseUInt32();

    return make_pair(function_name, function_code_index);
  }

  // used to get the bytecode when header has been parsed
  shared_ptr<uint8_t[]> getBytes(uint32_t count) {
    shared_ptr<uint8_t[]> bytes(new uint8_t[count]);
    std::memcpy(bytes.get(), buffer.get(), count * sizeof(uint8_t));
    return bytes;
  }

 public:
  DLVMModuleStreamParser(std::istream &input)
      : m_input{input},
        buffer(new uint8_t[input.gcount()]),
        constant_pool{std::make_shared<ConstantPool>()} {
    buffer_len = input.gcount();
    input.read(reinterpret_cast<char *>(buffer.get()), buffer_len);
  }

  // TODO: write some error handling
  std::optional<DLVMParsedModule> getModule() {
    uint32_t magic_number = parseUInt32();
    std::string module_name = parseString();
    uint32_t constant_pool_length = parseUInt32();
    uint32_t function_table_length = parseUInt32();
    uint32_t byte_code_length = parseUInt32();

    for (uint32_t i = 0; i < constant_pool_length; i++) {
      ConstantPoolEntryTypeTag type_tag =
          static_cast<ConstantPoolEntryTypeTag>(getByte());

      switch (type_tag) {
      MANAGED_FUNCTION_REF:
        constant_pool->addEntry(
            ConstantPoolEntry{type_tag, parseManagedFunctionRef()});
        break;
      NATIVE_FUNCTION_REF:
        constant_pool->addEntry(
            ConstantPoolEntry{type_tag, parseNativeFunctionRef()});
        break;
      STRING:
        constant_pool->addEntry(ConstantPoolEntry{type_tag, parseString()});
        break;
      UINT32:
        constant_pool->addEntry(ConstantPoolEntry{type_tag, parseUInt32()});
        break;
      UINT64:
        constant_pool->addEntry(ConstantPoolEntry{type_tag, parseUInt64()});
        break;
      INT64:
        constant_pool->addEntry(ConstantPoolEntry{type_tag, parseInt64()});
        break;
      DOUBLE:
        constant_pool->addEntry(ConstantPoolEntry{type_tag, parseDouble()});
        break;
      BOOL:
        constant_pool->addEntry(ConstantPoolEntry{type_tag, parseBool()});
        break;
        default:
          return std::nullopt;
      }
    }

    for (uint32_t i = 0; i < function_table_length; i++) {
      function_table->push_back(make_pair(parseString(), parseUInt32()));
    }

    auto byte_code = getBytes(byte_code_length);

    return std::make_optional(DLVMParsedModule{
        magic_number, module_name, constant_pool_length, function_table_length,
        byte_code_length, constant_pool, function_table, byte_code});
  }
};

class DLVMModuleTable {
 private:
  // (module_name, (function_name, global address of function))
  std::map<std::string, std::map<std::string, uint32_t>> m_module_table;

 public:
  DLVMModuleTable() : m_module_table{} {}

  void addModule(std::string module_name) {
    m_module_table[module_name] = std::map<std::string, uint32_t>{};
  }

  void addFunction(std::string module_name, std::string function_name,
                   uint32_t addr) {
    auto search = m_module_table.find(module_name);
    if (search == m_module_table.end()) {
      m_module_table[module_name] = std::map<std::string, uint32_t>{};
      m_module_table[module_name][function_name] = addr;
    } else {
      m_module_table[module_name][function_name] = addr;
    }
  }
};

class DLVMModuleLinker {
 private:
  std::shared_ptr<vector<DLVMParsedModule>> m_parsed_modules;
  std::shared_ptr<DLVMModuleTable> module_table;
  std::shared_ptr<ConstantPool> constant_pool;
  std::shared_ptr<uint8_t[]> byte_code;

  // bytecode includes both constan pool indices and program code
  // this is an index on the current position of the linker
  uint32_t byte_code_index = 0;

  void updateConstantPoolIndices(uint32_t constants_index,
                                 uint32_t constant_pool_length) {
    // add offset to constant pool indices
    for (uint32_t i = 0; i < constant_pool_length; i++) {
      uint32_t cpval;

      std::memcpy(&cpval,
                  &(byte_code.get()[constants_index + i * sizeof(uint32_t)]),
                  sizeof(uint32_t) / sizeof(uint8_t));

      cpval += constants_index;

      std::memcpy(&(byte_code.get()[constants_index + i * sizeof(uint32_t)]),
                  (uint8_t *)&cpval, sizeof(uint32_t) / sizeof(uint8_t));
    }
  }

 public:
  DLVMModuleLinker(std::shared_ptr<vector<DLVMParsedModule>> parsed_modules,
                   uint32_t program_length)
      : m_parsed_modules{parsed_modules},
        module_table{std::make_shared<DLVMModuleTable>()},
        constant_pool{std::make_shared<ConstantPool>()},
        byte_code{new uint8_t[program_length]} {}

  std::tuple<std::shared_ptr<DLVMModuleTable>, std::shared_ptr<ConstantPool>,
             std::shared_ptr<uint8_t[]>, uint32_t>
  linkModules() {
    for (auto &module : *m_parsed_modules) {
      module_table->addModule(module.module_name);

      // add module's constant pool to the linked program's constant pool
      constant_pool->addConstantPool(module.constant_pool);

      // move module's bytecode into linked program's bytecode
      std::memcpy(byte_code.get(), module.byte_code.get(),
                  module.byte_code_length);

      // global address for where the constant indices start
      uint32_t constants_index = byte_code_index;

      // global address for where the program code starts
      uint32_t program_index =
          byte_code_index + module.constant_pool_length * sizeof(uint32_t);

      updateConstantPoolIndices(constants_index, module.constant_pool_length);

      for (auto &function : *(module.function_table)) {
        auto [function_name, local_addr] = function;

        // add function to module table and set the address to the global
        // address of the module's program code plus the function's offset in
        // the module
        module_table->addFunction(module.module_name, function_name,
                                  program_index + local_addr);
      }

      byte_code_index += module.byte_code_length;
    }

    uint32_t byte_code_length = byte_code_index + 1;

    return make_tuple(module_table, constant_pool, byte_code, byte_code_length);
  }
};

}  // namespace module
}  // namespace dlvm
