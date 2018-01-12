pub mod interp {
    use vm::vm::*;
    use opcode::opcode::*;
    use ttype::ttype::*;

    impl<'a> VM<'a> {
        pub fn execute(&mut self) {
            let mut opcode: u8 = self.next();
            
            loop {    
                match opcode {
                    NOP         => { },
                    POP         => self.POP(),
                    PUSH_INT    => self.PUSH_INT(),
                    PUSH_FLOAT  => self.PUSH_FLOAT(),
                    ADD         => self.ADD(),
                    SUB         => self.SUB(),
                    MUL         => self.MUL(),
                    DIV         => self.DIV(),
                    PRINT       => self.PRINT(),
                    HALT        => break,
                    _           => break
                }

                opcode = self.next();
            } 
        }
    }
}