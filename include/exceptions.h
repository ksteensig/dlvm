#pragma once

typedef enum exception_type_e {
	IO				= 0x00,
	MEMORY			= 0x01,
	ARITHMETIC		= 0x02,
	DATA			= 0x03,
	TYPE			= 0x04,
	RUNTIME			= 0x05,
	CUSTOM			= 0xFF
} exception_type_t;

enum IO_exception {
	INPUT			= 0x00,
	OUTPUT			= 0x01
}

enum MEMORY_exception {
	OUT_OF_BOUND	= 0x00,
	NULL_EXCEPTION	= 0x01,
	OUT_OF_MEMORY	= 0x02
}

enum ARITHMETIC_exception {
	ZERO_DIVISION	= 0x00,
	FLOATING_ERROR	= 0x01
}

enum DATA_exception {
	
}
