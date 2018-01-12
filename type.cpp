#include "type.hpp"

namespace dlvm {

using namespace dlvm;
using namespace std;



shared_ptr<TBool> operator|| (const shared_ptr<TBool> lhs,
                            const shared_ptr<TBool> rhs) {
    return make_shared<TBool>(lhs->Bool || rhs->Bool);
}

shared_ptr<TBool> operator&& (const shared_ptr<TBool> lhs,
                            const shared_ptr<TBool> rhs) {
    return make_shared<TBool>(lhs->Bool && rhs->Bool);
}

shared_ptr<TBool> operator! (const shared_ptr<TBool> rhs) {
    return make_shared<TBool>(!rhs->Bool);
}

shared_ptr<TString> operator+ (const shared_ptr<TString> lhs,
                                const shared_ptr<TString> rhs) {
    return make_shared<TString>(lhs->String + rhs->String);
}

void TList::Append(Type &obj) {
    List.push_back(obj);
}

void TList::Prepend(Type &obj) {
    List.insert(List.begin(), obj);
}

void TList::Set(Type &obj, uint64_t pos) {
    List.at(pos) = obj;
}

Type &TList::Access(uint64_t pos) {
    return List.at(pos);
}

}