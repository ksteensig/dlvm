pub mod vm {
    use ttype::ttype::*;

    pub struct VM {
        program     : Vec<u8>,
        stack       : Vec<Type>,
        pagetable   : Vec<Option<u32>>,
        heap        : Vec<Type>,
        pub pc      : usize,
        pub sp      : usize,
        pub fp      : usize
    }

    impl VM {
        fn new(program: Vec<u8>) -> VM {
            let vm: VM = VM {
                program: program,
                stack: Vec::new(),
                pagetable: Vec::new(),
                heap: Vec::new(),
                pc: 0 as usize,
                sp: 0 as usize,
                fp: 0 as usize
            };

            vm
        }

        pub fn run(program: Vec<u8>) {
            let mut vm = VM::new(program);
            vm.execute();
        }

        pub fn pop(&mut self) -> Option<Type> {
            let val = self.stack.pop();
            self.sp -= 1;
            val
        }

        pub fn push(&mut self, val: Type) {
            self.stack.push(val);
            self.sp += 1
        }

        pub fn malloc() {
            
        }

        pub fn next(&mut self) -> u8 {
            let op = self.program[self.pc];
            self.pc += 1;
            op
        }

        pub fn get8(&mut self) -> u64 {
                let mut val: u64 = 0;
                for i in 0..8 {
                    val = (val << 8) | (self.next() as u64);
                }
                val
        }
    }
}