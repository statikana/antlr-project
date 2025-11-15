#pragma once
#include <cmath>

#include "../../koda_values.cpp"
#include "../visitor_base.cpp"

struct ArithPowVisitor : public VisitorBase {
	public:
	const Instance& rhs;
	ArithPowVisitor(const Instance& other) : VisitorBase(other), rhs(other) {}

	Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

	// integer power helper (non-negative exponent)
	static long long ipow(long long base, long long exp) {
		long long result = 1;
		while (exp > 0) {
			if (exp & 1) result *= base;
			base *= base;
			exp >>= 1;
		}
		return result;
	}

	Instance operator()(const BoolT lhs) {
		// bool as 0/1
		if (holds<BoolT>(rhs.data)) {
			// 0^0 or 1^1 -> 1, 1^0 ->1, etc. use integer pow
			IntT base = static_cast<IntT>(lhs);
			IntT exp  = static_cast<IntT>(get<BoolT>(rhs.data));
			return Instance::new_int(static_cast<IntT>(ipow(base, exp)));
		} else if (holds<IntT>(rhs.data)) {
			IntT base = static_cast<IntT>(lhs);
			IntT exp  = get<IntT>(rhs.data);
			if (exp >= 0) return Instance::new_int(static_cast<IntT>(ipow(base, exp)));
			return Instance::new_float(std::pow(static_cast<FloatT>(base), static_cast<FloatT>(exp)));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(std::pow(static_cast<FloatT>(lhs), get<FloatT>(rhs.data)));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const IntT lhs) {
		if (holds<BoolT>(rhs.data)) {
			IntT exp = static_cast<IntT>(get<BoolT>(rhs.data));
			if (exp >= 0) return Instance::new_int(static_cast<IntT>(ipow(lhs, exp)));
			return Instance::new_float(std::pow(static_cast<FloatT>(lhs), static_cast<FloatT>(exp)));
		} else if (holds<IntT>(rhs.data)) {
			IntT exp = get<IntT>(rhs.data);
			if (exp >= 0) return Instance::new_int(static_cast<IntT>(ipow(lhs, exp)));
			return Instance::new_float(std::pow(static_cast<FloatT>(lhs), static_cast<FloatT>(exp)));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(std::pow(static_cast<FloatT>(lhs), get<FloatT>(rhs.data)));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const FloatT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_float(std::pow(lhs, static_cast<FloatT>(get<BoolT>(rhs.data))));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_float(std::pow(lhs, static_cast<FloatT>(get<IntT>(rhs.data))));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(std::pow(lhs, get<FloatT>(rhs.data)));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const StringT lhs) {
		// string repetition: lhs ** n
		if (!holds<IntT>(rhs.data)) throw MSG_BAD_CALL;
		IntT times = get<IntT>(rhs.data);
		if (times <= 0) return Instance::new_string(StringT(""));
		StringT out;
		out.reserve(lhs.size() * static_cast<size_t>(times));
		for (IntT i = 0; i < times; ++i)
			out.append(lhs);
		return Instance::new_string(out);
	}

	Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; }
};
