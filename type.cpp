#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;

#pragma region // operations on numbers
template<class T>
unique_ptr<TNumber<double>> operator+ (const unique_ptr<TNumber<T>> lhs,
                                        const unique_ptr<TNumber<T>> rhs) {
    return make_unique<TNumber<T>>(lhs->Value + rhs->Value);
}

template<int64_t>
unique_ptr<TNumber<int64_t>> operator+ (const unique_ptr<TNumber<int64_t>> lhs,
                                        const unique_ptr<TNumber<int64_t>> rhs) {
    return make_unique<TNumber<int64_t>>(lhs->Value + rhs->Value);
}

template<class T>
unique_ptr<TNumber<double>> operator- (const unique_ptr<TNumber<T>> lhs,
                                        const unique_ptr<TNumber<T>> rhs) {
    return make_unique<TNumber<T>>(lhs->Value - rhs->Value);
}

template<int64_t>
unique_ptr<TNumber<int64_t>> operator- (const unique_ptr<TNumber<int64_t>> lhs,
                                        const unique_ptr<TNumber<int64_t>> rhs) {
    return make_unique<TNumber<int64_t>>(lhs->Value - rhs->Value);
}

template<class T>
unique_ptr<TNumber<double>> operator* (const unique_ptr<TNumber<T>> lhs,
                                        const unique_ptr<TNumber<T>> rhs) {
    return make_unique<TNumber<T>>(lhs->Value * rhs->Value);
}

template<int64_t>
unique_ptr<TNumber<int64_t>> operator* (const unique_ptr<TNumber<int64_t>> lhs,
                                        const unique_ptr<TNumber<int64_t>> rhs) {
    return make_unique<TNumber<int64_t>>(lhs->Value * rhs->Value);
}

template<class T>
unique_ptr<TNumber<double>> operator/ (const unique_ptr<TNumber<T>> lhs,
                                        const unique_ptr<TNumber<T>> rhs) {
    return make_unique<TNumber<T>>(lhs->Value / rhs->Value);
}

template<int64_t>
unique_ptr<TNumber<int64_t>> operator/ (const unique_ptr<TNumber<int64_t>> lhs,
                                        const unique_ptr<TNumber<int64_t>> rhs) {
    return make_unique<TNumber<int64_t>>(lhs->Value / rhs->Value);
}

template<int64_t>
unique_ptr<TNumber<int64_t>> operator% (const unique_ptr<TNumber<int64_t>> lhs,
                                        const unique_ptr<TNumber<int64_t>> rhs) {
    return make_unique<TNumber<int64_t>>(lhs->Value % rhs->Value);
}

#pragma endregion

unique_ptr<TBool> operator|| (const unique_ptr<TBool> lhs,
                            const unique_ptr<TBool> rhs) {
    return make_unique<TBool>(lhs->Bool || rhs->Bool);
}

unique_ptr<TBool> operator&& (const unique_ptr<TBool> lhs,
                            const unique_ptr<TBool> rhs) {
    return make_unique<TBool>(lhs->Bool && rhs->Bool);
}

unique_ptr<TBool> operator! (const unique_ptr<TBool> rhs) {
    return make_unique<TBool>(!rhs->Bool);
}

unique_ptr<TString> operator+ (const unique_ptr<TString> lhs,
                                const unique_ptr<TString> rhs) {
    return make_unique<TString>(lhs->String + rhs->String);
}

void TList::Append(unique_ptr<Type> obj) {
    List.push_back(move(obj));
}

void TList::Prepend(unique_ptr<Type> obj) {
    List.insert(List.begin(), move(obj));
}

void TList::Set(unique_ptr<Type> obj, uint64_t pos) {
    List.at(pos) = move(obj);
}

unique_ptr<Type> TList::Access(uint64_t pos) {
    return move(List.at(pos));
}

}