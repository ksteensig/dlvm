pub mod ttype {
    use std::ops::*;

    #[derive(Debug)]
    pub enum Error {
        Error,
        //ErrorDescription(Box<String>)
    }

    #[derive(Debug)]
    pub enum Type {
        TInteger(i64, u32),
        TFloat(f64, u32),
        TBool(bool, u32),
        TChar(char, u32),
        TReference(u32),
        TArray(u32, u32),
        TError(Box<Error>)
    }

    impl<'a> Add for Type {
        type Output = Type;

        fn add(self, other: Type) -> Type {
            match (self, other) {
                (Type::TInteger(v1), Type::TInteger(v2))    => Type::TInteger(v1 + v2),
                (Type::TFloat(v1), Type::TFloat(v2))        => Type::TFloat(v1 + v2),
                (Type::TInteger(v1), Type::TFloat(v2))      => Type::TFloat((v1 as f64) + v2),
                (Type::TFloat(v1), Type::TInteger(v2))      => Type::TFloat(v1 + (v2 as f64)),
                _                                           => Type::TError(Error::Error)
            }
        }
    }

    impl<'a> Sub for Type {
        type Output = Type;

        fn sub(self, other: Type) -> Type {
            match (self, other) {
                (Type::TInteger(v1), Type::TInteger(v2))    => Type::TInteger(v1 + v2),
                (Type::TFloat(v1), Type::TFloat(v2))        => Type::TFloat(v1 + v2),
                (Type::TInteger(v1), Type::TFloat(v2))      => Type::TFloat((v1 as f64) + v2),
                (Type::TFloat(v1), Type::TInteger(v2))      => Type::TFloat(v1 + (v2 as f64)),
                _                                           => Type::TError(Error::Error)
            }
        }
    }

    impl<'a> Mul for Type {
        type Output = Type;

        fn mul(self, other: Type) -> Type {
            match (self, other) {
                (Type::TInteger(v1), Type::TInteger(v2))    => Type::TInteger(v1 + v2),
                (Type::TFloat(v1), Type::TFloat(v2))        => Type::TFloat(v1 + v2),
                (Type::TInteger(v1), Type::TFloat(v2))      => Type::TFloat((v1 as f64) + v2),
                (Type::TFloat(v1), Type::TInteger(v2))      => Type::TFloat(v1 + (v2 as f64)),
                _                                           => Type::TError(Error::Error)
            }
        }
    }

    impl<'a> Div for Type {
        type Output = Type;

        fn div(self, other: Type) -> Type {
            match (self, other) {
                (Type::TInteger(v1), Type::TInteger(v2))    => Type::TInteger(v1 + v2),
                (Type::TFloat(v1), Type::TFloat(v2))        => Type::TFloat(v1 + v2),
                (Type::TInteger(v1), Type::TFloat(v2))      => Type::TFloat((v1 as f64) + v2),
                (Type::TFloat(v1), Type::TInteger(v2))      => Type::TFloat(v1 + (v2 as f64)),
                _                                           => Type::TError(Error::Error)
            }
        }
    }

    impl<'a> BitAnd for Type {
        type Output = Type;
        fn bitand(self, other: Type) -> Type {
            match(self, other) {
                (Type::TBool(b1), Type::TBool(b2))  => Type::TBool(b1 & b2), 
                _                       => Type::TError(Error::Error)
            }
        }
    }

    impl<'a> BitOr for Type {
        type Output = Type;
        fn bitor(self, other: Type) -> Type {
            match(self, other) {
                (Type::TBool(b1), Type::TBool(b2))  => Type::TBool(b1 | b2), 
                _                       => Type::TError(Error::Error)
            }
        }
    }

    impl<'a> Not for Type {
        type Output = Type;
        fn not(self) -> Type {
            match self {
                Type::TBool(b)  => Type::TBool(!b), 
                _               => Type::TError(Error::Error)
            }
        }
    }
}