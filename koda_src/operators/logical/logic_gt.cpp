#pragma once
#include "../visitor_base.cpp"
#include "../../koda_values.cpp"


class LogicGtVisitor : public VisitorBase {
  public:
    const Instance& rhs;
    LogicGtVisitor(const Instance& other) : VisitorBase(other), rhs(other) {}
    
    Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const BoolT lhs) {
        if (holds<BoolT>(rhs.data)) {
            return Instance::new_bool(lhs > get<BoolT>(rhs.data));
        } else if (holds<IntT>(rhs.data)) {
            return Instance::new_bool(static_cast<int>(lhs) > get<IntT>(rhs.data));
        } else if (holds<FloatT>(rhs.data)) {
            return Instance::new_bool(static_cast<float>(lhs) > get<FloatT>(rhs.data));
        }
        throw MSG_BAD_CALL;
    }

    Instance operator()(const IntT lhs) {
        if (holds<IntT>(rhs.data)) {
            return Instance::new_bool(lhs > get<IntT>(rhs.data));
        } else if (holds<BoolT>(rhs.data)) {
            return Instance::new_bool(lhs > static_cast<int>(get<BoolT>(rhs.data)));
        } else if (holds<FloatT>(rhs.data)) {
            return Instance::new_bool(static_cast<float>(lhs) > get<FloatT>(rhs.data));
        }
        throw MSG_BAD_CALL;
    }

    Instance operator()(const FloatT lhs) {
        if (holds<FloatT>(rhs.data)) {
            return Instance::new_bool(lhs > get<FloatT>(rhs.data));
        } else if (holds<IntT>(rhs.data)) {
            return Instance::new_bool(lhs > static_cast<float>(get<IntT>(rhs.data)));
        } else if (holds<BoolT>(rhs.data)) {
            return Instance::new_bool(lhs > static_cast<float>(get<BoolT>(rhs.data)));
        }
        throw MSG_BAD_CALL;
    }

    Instance operator()(const StringT lhs) {
        if (holds<StringT>(rhs.data)) {
            return Instance::new_bool(lhs > get<StringT>(rhs.data));
        }
        throw MSG_BAD_CALL;
    }

    Instance operator()(const StructData lhs) {
        throw MSG_BAD_CALL;
    }

    Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }
};