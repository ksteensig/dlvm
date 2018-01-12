pub mod vm {
    use ttype::ttype::*;

    pub struct VM<'a> {
        program : &'a Vec<u8>,
        stack   : &'a mut Vec<Type<'a>>,
        sp      : usize,
        pc      : usize
    }

    impl<'a> VM<'a> {
        fn new(program: &'a Vec<u8>, stack: &'a mut Vec<Type<'a>>) -> VM<'a> {
            let vm: VM<'a> = VM {
                program: program,
                stack: stack,
                sp: 0 as usize,
                pc: 0 as usize
            };

            vm
        }

        pub fn run(program: &'a Vec<u8>) {
            let mut vm = &mut VM {
                program: program,
                stack: &mut Vec::new(),
                sp: 0 as usize,
                pc: 0 as usize
            };

            vm.execute();
        }

        fn pop(&mut self) -> Option<Type<'a>> {
            let val = self.stack.pop();
            self.sp -= 1;
            val
        }

        fn push(&mut self, val: Type<'a>) {
            self.stack.push(val);
            self.sp += 1
        }

        pub fn next(&mut self) -> u8 {
            let op = self.program[self.pc];
            self.pc += 1;
            op
        }

        pub fn POP(&mut self) {
            self.pop();
        }

        pub fn PUSH_INT(&mut self) {
            let mut value: u64 = 0;

            for i in 0..8 {
                value = (value << 8) | (self.next() as u64);
            }

            self.push(Type::TInteger(value as i64));
        }

        pub fn PUSH_FLOAT(&mut self) {
            let mut value: u64 = 0;

            for i in 0..8 {
                value = (value << 8) | (self.next() as u64);
            }

            self.push(Type::TFloat(value as f64));
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

        pub fn PRINT(&mut self) {
            let v = self.pop();

            match v {
                Some(val)   => println!("{:?}", val),
                _           => {}
            }
        }
    }
}