#pragma once
#include "../../koda_values.cpp"
#include "../visitor_base.cpp"

// Division always uses FloatT (float), no integer truncation
struct ArithDivVisitor : public VisitorBase {
	public:
	const Instance& rhs;
	ArithDivVisitor(const Instance& other) : VisitorBase(other), rhs(other) {}

	Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const BoolT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) / static_cast<FloatT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) / static_cast<FloatT>(get<IntT>(rhs.data)));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) / get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const IntT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) / static_cast<FloatT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) / static_cast<FloatT>(get<IntT>(rhs.data)));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) / get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const FloatT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) / static_cast<FloatT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) / static_cast<FloatT>(get<IntT>(rhs.data)));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(lhs / get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const StringT lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; }
};
