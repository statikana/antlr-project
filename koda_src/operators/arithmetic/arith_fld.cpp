#pragma once
#include <cmath>

#include "../../koda_values.cpp"
#include "../visitor_base.cpp"

struct ArithFldVisitor : public VisitorBase {
	public:
	const Instance& rhs;
	ArithFldVisitor(const Instance& other) : VisitorBase(other), rhs(other) {}

	Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const BoolT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_int(static_cast<IntT>(lhs) / static_cast<IntT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_int(static_cast<IntT>(lhs / get<IntT>(rhs.data)));
		} else if (holds<FloatT>(rhs.data)) {
			FloatT res = static_cast<FloatT>(lhs) / get<FloatT>(rhs.data);
			return Instance::new_int(static_cast<IntT>(std::floor(res)));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const IntT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_int(lhs / static_cast<IntT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_int(lhs / get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			FloatT res = static_cast<FloatT>(lhs) / get<FloatT>(rhs.data);
			return Instance::new_int(static_cast<IntT>(std::floor(res)));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const FloatT lhs) {
		if (holds<BoolT>(rhs.data)) {
			FloatT res = lhs / static_cast<FloatT>(get<BoolT>(rhs.data));
			return Instance::new_int(static_cast<IntT>(std::floor(res)));
		} else if (holds<IntT>(rhs.data)) {
			FloatT res = lhs / static_cast<FloatT>(get<IntT>(rhs.data));
			return Instance::new_int(static_cast<IntT>(std::floor(res)));
		} else if (holds<FloatT>(rhs.data)) {
			FloatT res = lhs / get<FloatT>(rhs.data);
			return Instance::new_int(static_cast<IntT>(std::floor(res)));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const StringT lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; }
};
