#pragma once

typedef enum header_bytes_e {
	NOP					= 0x00,
	ADD_FUNCTION		= 0x01,
	SET_ENTRYPOINT		= 0x02,
	END_HEADER			= 0xFF	
} header_bytes_t;

typedef enum bytecode_e {
    NOP                 = 0x00,
    ADD                 = 0x01,
    SUB                 = 0x02,
    MUL                 = 0x03,
    DIV                 = 0x04,
    MOD                 = 0x05,
    PUSH                = 0x06,
    POP                 = 0x07,
    INSERT_LIST         = 0x08,
    SET_LIST            = 0x09,
    ACCESS_LIST         = 0x0A,
    CALL                = 0x0B,
    RET                 = 0x0C,
    INVOKE              = 0x0D,
    LOAD                = 0x0E,
    GLOAD               = 0x0F,
    AND                 = 0x10,
    OR                  = 0x11,
    NOT                 = 0x12,
    JMP                 = 0x13,
    JMPF                = 0x14,
    JMPT                = 0x15,
    CMP_EQ              = 0x16,
    CMP_L               = 0x17,
    CMP_G               = 0x18,
    CMP_LE              = 0x19,
    CMP_GE              = 0x1A,
    COPY                = 0x1B,
	RAISE				= 0x1C,
	TRY_CATCH			= 0x1D,
    PRINT               = 0xFE,
    HALT                = 0xFF
} bytecode_t;
