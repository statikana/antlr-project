#pragma once
#include "../visitor_base.cpp"
#include "../../koda_values.cpp"


class LogicNotVisitor : public VisitorBase {
  public:
    const Instance& rhs;
    LogicNotVisitor(const Instance& other) : VisitorBase(other), rhs(other) {}

    Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const BoolT lhs) {
        return Instance::new_bool(!lhs);
    }

    Instance operator()(const IntT lhs) {
        return Instance::new_bool(lhs == 0);
    }

    Instance operator()(const FloatT lhs) {
        return Instance::new_bool(lhs == 0.0);
    }

    Instance operator()(const StringT lhs) {
        return Instance::new_bool(lhs.empty());
    }

    Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }
};