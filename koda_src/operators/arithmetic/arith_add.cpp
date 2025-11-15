#pragma once
#include "../../koda_values.cpp"
#include "../visitor_base.cpp"

struct ArithAddVisitor : public VisitorBase {
	public:
	const Instance& rhs;
	ArithAddVisitor(const Instance& other) : VisitorBase(other), rhs(other) {}

	Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const BoolT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_int(lhs + static_cast<IntT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_int(lhs + get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) + get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const IntT lhs) {
		if (holds<VoidT>(rhs.data)) {
			throw MSG_BAD_CALL;
		} else if (holds<BoolT>(rhs.data)) {
			return Instance::new_int(lhs + static_cast<IntT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_int(lhs + get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) + get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const FloatT lhs) {
		if (holds<VoidT>(rhs.data)) {
			throw MSG_BAD_CALL;
		} else if (holds<BoolT>(rhs.data)) {
			return Instance::new_float(lhs + static_cast<FloatT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_float(lhs + get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(lhs + get<FloatT>(rhs.data));
		} else if (holds<StringT>(rhs.data)) {
			return Instance::new_string(std::to_string(lhs) + get<StringT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const StringT lhs) { return Instance::new_string(lhs + rhs.to_text()); }

	Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }
};
