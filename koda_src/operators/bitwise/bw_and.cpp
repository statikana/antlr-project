#include "../visitor_base.cpp"
#include "../../koda_values.cpp"

class BwAndVisitor : public VisitorBase {
  public:
    const Instance& rhs;
    BwAndVisitor(const Instance& other) : VisitorBase(other), rhs(other) {}

    Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const BoolT lhs) {
        if (holds<BoolT>(rhs.data)) {
            return Instance::new_bool(lhs && get<BoolT>(rhs.data));
        }
        throw MSG_BAD_CALL;
    }

    Instance operator()(const IntT lhs) {
        if (holds<BoolT>(rhs.data)) {
            return Instance::new_int(lhs & get<BoolT>(rhs.data));
        } else if (holds<IntT>(rhs.data)) {
            return Instance::new_int(lhs & get<IntT>(rhs.data));
        }
        throw MSG_BAD_CALL;
    }

    Instance operator()(const FloatT lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const StringT lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; }

    Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }
};