pub mod opcode {
    pub const NOP: u8 = 0x00;
    pub const POP: u8  = 0x01;
    pub const PUSH_INT: u8 = 0x02;
    pub const PUSH_FLOAT: u8 = 0x03;
    pub const ADD: u8 = 0x10;
    pub const SUB: u8 = 0x11;
    pub const MUL: u8 = 0x12;
    pub const DIV: u8 = 0x13;
    pub const PRINT: u8 = 0xFE;
    pub const HALT: u8 = 0xFF;
}