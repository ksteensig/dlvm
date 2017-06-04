/*
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
*/

typedef enum opcode_e {
    NOP = 0x00,
    ADD = 0x01,
    MUL = 0x02,
    PUSH = 0x03,
    POP = 0x04,
    PRINT = 0x05,
    HALT = 0xFF,
} opcode_t;