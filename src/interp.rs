pub mod interp {
    use vm::vm::*;
    use vm_handler::vm_handler::*;
    use opcode::opcode::*;
    use ttype::ttype::*;

    impl VM {
        pub fn execute(&mut self) {
            let mut opcode: u8 = self.next();
            
            loop {    
                match opcode {
                    NOP         => { },
                    POP         => self.POP(),
                    PUSH_INT    => self.PUSH_INT(),
                    PUSH_FLOAT  => self.PUSH_FLOAT(),
                    PUSH_BOOL   => self.PUSH_BOOL(),
                    PUSH_STRING => self.PUSH_STRING(),
                    PUSH_LIST   => self.PUSH_LIST(),
                    ADD         => self.ADD(),
                    SUB         => self.SUB(),
                    MUL         => self.MUL(),
                    DIV         => self.DIV(),
                    AND         => self.AND(),
                    OR          => self.OR(),
                    NOT         => self.NOT(),
                    PRINT       => self.PRINT(),
                    HALT        => break,
                    _           => break
                }

                opcode = self.next();
            } 
        }
    }
}