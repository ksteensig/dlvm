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
    SUB = 0x02,
    MUL = 0x03,
    DIV = 0x04,
    MOD = 0x05,
    PUSH = 0x06,
    POP = 0x07,
    CREATE_LIST = 0x08,
    INSERT_LIST = 0x09,
    SET_LIST = 0x0A,
    ACCESS_LIST = 0x0B,
    PRINT = 0xFE,
    HALT = 0xFF,
} opcode_t;