pub mod vm_handler {
    use vm::vm::*;
    use ttype::ttype::*;
    use std::char;

    impl VM {
        pub fn POP(&mut self) {
            self.pop();
        }

        pub fn PUSH_INT(&mut self) {
            let v: u64 = self.get8();
            self.push(Type::TInteger(v as i64));
        }

        pub fn PUSH_FLOAT(&mut self) {
            let mut value: u64 = 0;

            for i in 0..8 {
                value = (value << 8) | (self.next() as u64);
            }

            self.push(Type::TFloat(value as f64));
        }

        pub fn PUSH_BOOL(&mut self) {
            match self.next() {
                0 => self.push(Type::TBool(false)),
                _ => self.push(Type::TBool(true))
            }
        }

        pub fn PUSH_STRING(&mut self) {
            let mut str: Vec<char> = Vec::new();
            
            {
            let mut get_char = || -> Option<char> {
                let mut value: u32 = 0;
                
                for i in 0..4 {
                    value = (value << 8) | (self.next() as u32);
                }

                char::from_u32(value)
            };

            loop {
                match get_char() {
                    Some('\0')  => {str.push('\0'); break},
                    Some(c)     => str.push(c),
                    _           => break
                }
            }
            }

            self.push(Type::TString(str.into_iter().collect()))
        }

        pub fn PUSH_LIST(&mut self) {
            self.push(Type::TList(Vec::new()))
        }

        pub fn ADD(&mut self) {
            let v1 = self.pop();
            let v2 = self.pop();

            match (v1, v2) {
                (Some(n1), Some(n2)) => self.push(n1+n2),
                _                    => { }
            }
        }

        pub fn SUB(&mut self) {
            let v1 = self.pop();
            let v2 = self.pop();

            match (v1, v2) {
                (Some(n1), Some(n2)) => self.push(n1-n2),
                _                    => { }
            }
        }

        pub fn MUL(&mut self) {
            let v1 = self.pop();
            let v2 = self.pop();

            match (v1, v2) {
                (Some(n1), Some(n2)) => self.push(n1*n2),
                _                    => { }
            }
        }

        pub fn DIV(&mut self) {
            let v1 = self.pop();
            let v2 = self.pop();

            match (v1, v2) {
                (Some(n1), Some(n2)) => self.push(n1/n2),
                _                    => { }
            }
        }

        pub fn AND(&mut self) {
            let v1 = self.pop();
            let v2 = self.pop();

            match (v1, v2) {
                (Some(b1), Some(b2))    => self.push(b1&b2),
                _                       => { }
            }
        }

        pub fn OR(&mut self) {
            let v1 = self.pop();
            let v2 = self.pop();

            match (v1, v2) {
                (Some(b1), Some(b2))    => self.push(b1|b2),
                _                       => { }
            }
        }

        pub fn NOT(&mut self) {
            let v = self.pop();

            match v {
                Some(b)     => self.push(!b),
                _           => { }
            }
        }
/*
        pub fn CALL(&mut self) {
            let args = self.get8();
            let pc = self.get8();
            self.push(Type::TInteger(args as i64));
            self.push(Type::TInteger(&self.fp as i64));
            self.push(Type::TInteger(&self.pc as i64));
            self.fp = self.sp;
            self.pc = (pc as usize);
        }

        pub fn RET(&mut self) {
            let result = self.pop();
            let pc = self.pop();
            let fp = self.pop();
            let sp_offset = self.pop();

            match (result, pc, fp, sp_offset) {
                (Some(r), Some(Type::TInteger(p)),
                    Some(Type::TInteger(f)),
                    Some(Type::TInteger(s))) => {
                    
                    self.sp = self.fp;
                    self.pc = (p as usize);
                    self.fp = (f as usize);
                    self.sp -= (s as usize);
                    self.push(r);
                },
                _         => { }
            }
        }
*/
        pub fn PRINT(&mut self) {
            let v = self.pop();

            match v {
                Some(val)   => println!("{:?}", val),
                _           => {}
            }
        }
    }
}