#pragma once
#include "../../koda_values.cpp"
#include "../visitor_base.cpp"

struct ArithSubVisitor : public VisitorBase {
	public:
	const Instance& rhs;
	ArithSubVisitor(const Instance& other) : VisitorBase(other), rhs(other) {}

	Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const BoolT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_int(static_cast<IntT>(lhs) - static_cast<IntT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_int(lhs - get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) - get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}
	
	Instance operator()(const IntT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_int(lhs - static_cast<IntT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_int(lhs - get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(static_cast<FloatT>(lhs) - get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const FloatT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_float(lhs - static_cast<FloatT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_float(lhs - static_cast<FloatT>(get<IntT>(rhs.data)));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(lhs - get<FloatT>(rhs.data));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const StringT lhs) {
		if (!holds<StringT>(rhs.data)) {
			throw MSG_BAD_CALL;
		}

		// string truncation: "foobar" - "bar"  = "foo"
		// for clarity, only defined for str - str (unlike string concatenation,
		// where "foo" + 5 = "foo5")
		StringT rhs_str = get<StringT>(rhs.data);

		auto start = lhs.find(rhs_str);
		if (start == StringT::npos) {
			return Instance::new_string(lhs);
		} else {
			return Instance::new_string(lhs.substr(0, start));
		}
	}

	Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; }
};
