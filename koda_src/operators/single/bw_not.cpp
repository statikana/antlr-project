#pragma once
#include "../visitor_base.cpp"
#include "../../koda_values.cpp"


// Call with RHS is void
class BwNotVisitor : public VisitorBase {
  public:
    const Instance& rhs;
    BwNotVisitor(const Instance& other) : VisitorBase(other), rhs(other) {}

    Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const BoolT lhs) {
        return Instance::new_bool(!lhs);
    }

    Instance operator()(const IntT lhs) {
        return Instance::new_int(~lhs);
    }

    Instance operator()(const FloatT lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const StringT lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }
};