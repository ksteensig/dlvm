pub mod ttype {
    use std::ops::Add;
    use std::ops::Sub;
    use std::ops::Mul;
    use std::ops::Div;

    #[derive(Debug)]
    pub enum Error<'a> {
        Error,
        ErrorDescription(&'a String)
    }

    #[derive(Debug)]
    pub enum Type<'a> {
        TInteger     (i64),
        TFloat(f64 ),
        TBool(bool),
        TString(&'a mut String),
        TList(&'a mut Vec<Type<'a>>),
        TError(Error<'a>)
    }

    impl<'a> Add for Type<'a> {
        type Output = Type<'a>;

        fn add(self, other: Type<'a>) -> Type<'a> {
            match (self, other) {
                (Type::TInteger(v1), Type::TInteger(v2))    => Type::TInteger(v1 + v2),
                (Type::TFloat(v1), Type::TFloat(v2))        => Type::TFloat(v1 + v2),
                (Type::TInteger(v1), Type::TFloat(v2))      => Type::TFloat((v1 as f64) + v2),
                (Type::TFloat(v1), Type::TInteger(v2))      => Type::TFloat(v1 + (v2 as f64)),
                _                                           => Type::TError(Error::Error)
            }
        }
    }

    impl<'a> Sub for Type<'a> {
        type Output = Type<'a>;

        fn sub(self, other: Type<'a>) -> Type<'a> {
            match (self, other) {
                (Type::TInteger(v1), Type::TInteger(v2))    => Type::TInteger(v1 + v2),
                (Type::TFloat(v1), Type::TFloat(v2))        => Type::TFloat(v1 + v2),
                (Type::TInteger(v1), Type::TFloat(v2))      => Type::TFloat((v1 as f64) + v2),
                (Type::TFloat(v1), Type::TInteger(v2))      => Type::TFloat(v1 + (v2 as f64)),
                _                                           => Type::TError(Error::Error)
            }
        }
    }

    impl<'a> Mul for Type<'a> {
        type Output = Type<'a>;

        fn mul(self, other: Type<'a>) -> Type<'a> {
            match (self, other) {
                (Type::TInteger(v1), Type::TInteger(v2))    => Type::TInteger(v1 + v2),
                (Type::TFloat(v1), Type::TFloat(v2))        => Type::TFloat(v1 + v2),
                (Type::TInteger(v1), Type::TFloat(v2))      => Type::TFloat((v1 as f64) + v2),
                (Type::TFloat(v1), Type::TInteger(v2))      => Type::TFloat(v1 + (v2 as f64)),
                _                                           => Type::TError(Error::Error)
            }
        }
    }

    impl<'a> Div for Type<'a> {
        type Output = Type<'a>;

        fn div(self, other: Type<'a>) -> Type<'a> {
            match (self, other) {
                (Type::TInteger(v1), Type::TInteger(v2))    => Type::TInteger(v1 + v2),
                (Type::TFloat(v1), Type::TFloat(v2))        => Type::TFloat(v1 + v2),
                (Type::TInteger(v1), Type::TFloat(v2))      => Type::TFloat((v1 as f64) + v2),
                (Type::TFloat(v1), Type::TInteger(v2))      => Type::TFloat(v1 + (v2 as f64)),
                _                                           => Type::TError(Error::Error)
            }
        }
    }
}