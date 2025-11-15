#pragma once
#include "../../koda_values.cpp"
#include "../visitor_base.cpp"

struct ArithMulVisitor {
	public:
	const Instance& rhs;
	ArithMulVisitor(const Instance& other) : rhs(other) {}

	Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const BoolT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_int(static_cast<IntT>(lhs) * static_cast<IntT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_int(static_cast<IntT>(lhs) * get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) * get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const IntT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_int(lhs * static_cast<IntT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_int(lhs * get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) * get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const FloatT lhs) {
		if (holds<IntT>(rhs.data)) {
			return Instance::new_float(lhs * get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(lhs * get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const StringT lhs) {
		if (holds<IntT>(rhs.data)) {
			auto lim		= get<IntT>(rhs.data);
			StringT current = "";
			for (size_t i = 0; i < lim; i++) {
				current.append(lhs);
			}
			return Instance::new_string(current);
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; }
};
