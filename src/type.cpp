#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

ReferenceType ValueType::Box() { return ReferenceType{type, Value}; }

Result<ValueType> ReferenceType::Unbox() {
  switch (type) {
    case ARRAY:
    case STRING:
    case STRUCT:
      return LiftError<ValueType>(INVALID_ARGUMENT, "");
    default:
      return LiftType(ValueType{type, get<VType>(Value)});
  }
}

/*
Either operator+ (Result lhs, Result rhs) {
    if (lhs.ErrCode != OK || rhs.ErrCode != OK) {
        return ThrowError("Invalid value", INVALID_ARGUMENT);
    }

    if (lhs.Value.type == INTEGER && rhs.Value.type == INTEGER) {
        return OkResult(
                Type{INTEGER,
                    get<int64_t>(lhs.Value.Value) +
                    get<int64_t>(rhs.Value.Value)}
            );
    } else {
        return TypeError(Type{});
    }
}
*/
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
Either          BOOL,
                !get<bool>(rhs.Value)
            );
    } else {
        return TypeError<Type>(BOOL, false);
    }
}
*/
/*
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
*/

}  // namespace dlvm
