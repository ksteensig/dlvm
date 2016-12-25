#ifndef LISP_LANG_OPCODES_H
#define LISP_LANG_OPCODES_H

typedef enum opcode_e {
    NOP = 0x00,
    ADD = 0x01,
    MUL = 0x02,
    POP = 0x03,
    PUSH = 0x04,
    GLOAD = 0x05,
    LOAD  = 0x06,
    CALL = 0x07,
    RET = 0x08,
    PRINT = 0x09,
    MAKEARRAY = 0x0A,
    SETARRAY = 0x0B,
    ACCESSARRAY = 0x0C,
    HALT = 0xFF,
} opcode_t;

static uint32_t parameter_table[] {
    0,
    0,
    0,
    1,
    0,

};

#endif //LISP_LANG_OPCODES_H
