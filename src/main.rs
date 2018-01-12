pub mod ttype;
pub mod vm;
pub mod opcode;
pub mod interp;

use opcode::opcode::*;
use vm::vm::*;
use ttype::ttype::*;

fn main() {
    let program: &Vec<u8> = &vec![
        PUSH_INT,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
        PUSH_INT,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
        PRINT,
        HALT
    ];
    
    VM::run(&program);

    println!("Hello, world!");
}
