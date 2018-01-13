#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;
using namespace std::literals;

template<class T>
Result<T> OkResult(type_t type, TypeCollection value) {
    return Result<T> {
            Type {
                type,
                value
            }, "", OK,
        };
}

template<class T>
Result<T> TypeError(type_t type, TypeCollection value) {
    return Result<T> { 
        Type { type, value }, "Type error", TYPE_ERROR
    };
}

Result<Type> operator+ (Type lhs, Type rhs) {
    if (lhs.type == INTEGER && rhs.type == INTEGER) {
        return OkResult<Type>(
                INTEGER,
                get<int64_t>(lhs.Value) + get<int64_t>(rhs.Value)
            );
    } else if (lhs.type == FLOAT && rhs.type == FLOAT) {
        return OkResult<Type>(
            FLOAT,
            get<double>(lhs.Value) + get<double>(rhs.Value)
        );
    } else if (lhs.type == FLOAT && rhs.type == INTEGER) {
        return OkResult<Type>(
            FLOAT,
            get<double>(lhs.Value) + get<int64_t>(rhs.Value)
        );
    } else if (lhs.type == INTEGER && rhs.type == FLOAT) {
        return OkResult<Type>(
            FLOAT,
            get<int64_t>(lhs.Value) + get<double>(rhs.Value)
        );
    } else {
        return TypeError<Type>(INTEGER, (int64_t)0);
    }
}
/*
Result<Type> operator- (Type lhs, Type rhs) {
    if (lhs.type == INTEGER && rhs.type == INTEGER) {
        return OkResult<Type>(
            INTEGER,
            get<int64_t>(lhs.Value) - get<int64_t>(rhs.Value)
        );
    } else if (lhs.type == FLOAT && rhs.type == FLOAT) {
        return OkResult<Type>(
            FLOAT,
            get<double>(lhs.Value) - get<double>(rhs.Value)
        );
    } else if (lhs.type == FLOAT && rhs.type == INTEGER) {
        return OkResult<Type>(
            FLOAT,
            get<double>(lhs.Value) - get<int64_t>(rhs.Value)
        );
    } else if (lhs.type == INTEGER && rhs.type == FLOAT) {
        return OkResult<Type>(
            FLOAT,
            get<int64_t>(lhs.Value) - get<double>(rhs.Value)
        );
    } else {
        return TypeError<Type>(INTEGER, (int64_t)0);
    }
}

Result<Type> operator* (Type lhs, Type rhs) {
    if (lhs.type == INTEGER && rhs.type == INTEGER) {
        return OkResult<Type>(
            INTEGER,
            get<int64_t>(lhs.Value) * get<int64_t>(rhs.Value)
        );
    } else if (lhs.type == FLOAT && rhs.type == FLOAT) {
        return OkResult<Type>(
            FLOAT,
            get<double>(lhs.Value) * get<double>(rhs.Value)
        );
    } else if (lhs.type == FLOAT && rhs.type == INTEGER) {
        return OkResult<Type>(
            FLOAT,
            get<double>(lhs.Value) * get<int64_t>(rhs.Value)
        );
    } else if (lhs.type == INTEGER && rhs.type == FLOAT) {
        return OkResult<Type>(
            FLOAT,
            get<int64_t>(lhs.Value) * get<double>(rhs.Value)
        );
    } else {
        return TypeError<Type>(INTEGER, (int64_t)0);
    }
}

Result<Type> operator/ (Type lhs, Type rhs) {
    if (lhs.type == INTEGER && rhs.type == INTEGER) {
        return OkResult<Type>(
            INTEGER,
            get<int64_t>(lhs.Value) / get<int64_t>(rhs.Value)
        );
    } else if (lhs.type == FLOAT && rhs.type == FLOAT) {
        return OkResult<Type>(
            FLOAT,
            get<double>(lhs.Value) / get<double>(rhs.Value)
        );
    } else if (lhs.type == FLOAT && rhs.type == INTEGER) {
        return OkResult<Type>(
            FLOAT,
            get<double>(lhs.Value) / get<int64_t>(rhs.Value)
        );
    } else if (lhs.type == INTEGER && rhs.type == FLOAT) {
        return OkResult<Type>(
            FLOAT,
            get<int64_t>(lhs.Value) / get<double>(rhs.Value)
        );
    } else {
        return TypeError<Type>(INTEGER, (int64_t)0);
    }
}

Result<Type> operator&& (Type lhs, Type rhs) {
    if (lhs.type == BOOL && rhs.type == BOOL) {
        return OkResult<Type>(
                BOOL,
                get<bool>(lhs.Value) && get<bool>(rhs.Value)
            );
    } else {
        return TypeError<Type>(BOOL, false);
    }
}

Result<Type> operator|| (Type lhs, Type rhs) {
    if (lhs.type == BOOL && rhs.type == BOOL) {
        return OkResult<Type>(
                BOOL,
                get<bool>(lhs.Value) || get<bool>(rhs.Value)
            );
    } else {
        return TypeError<Type>(BOOL, false);
    }
}

Result<Type> operator! (Type rhs) {
    if (rhs.type == BOOL) {
        return OkResult<Type>(
                BOOL,
                !get<bool>(rhs.Value)
            );
    } else {
        return TypeError<Type>(BOOL, false);
    }
}
*/
ostream& operator<< (ostream &o, Type t) {
    switch (t.type) {
        case INTEGER:
            o << get<int64_t>(t.Value);
            break;
        default:
            break;
    }

    return o;
}

}