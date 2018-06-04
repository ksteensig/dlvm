#pragma once

namespace dlvm {

typedef enum opcode_e {
  NOP = 0x00,
  POP = 0x01,
  PUSH_INT = 0x02,
  PUSH_FLOAT = 0x03,
  ADD = 0x10,
  SUB = 0x11,
  MUL = 0x12,
  DIV = 0x13,
  LIST_CREATE = 0x20,
  LIST_ACCESS = 0x21,
  LIST_SET = 0x22,
  CREATE_REFERENCE = 0x23,
  PRINT,
  HALT
} opcode_t;
}
