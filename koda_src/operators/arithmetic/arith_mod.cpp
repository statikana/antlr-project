#pragma once
#include <cmath>

#include "../../koda_values.cpp"
#include "../visitor_base.cpp"

struct ArithModVisitor : public VisitorBase {
	public:
	const Instance& rhs;
	ArithModVisitor(const Instance& other) : VisitorBase(other), rhs(other) {}

	Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const BoolT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_int(static_cast<IntT>(lhs) % static_cast<IntT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_int(static_cast<IntT>(lhs) % get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(std::fmod(static_cast<FloatT>(lhs), get<FloatT>(rhs.data)));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const IntT lhs) {
		if (holds<BoolT>(rhs.data)) {
			return Instance::new_int(lhs % static_cast<IntT>(get<BoolT>(rhs.data)));
		} else if (holds<IntT>(rhs.data)) {
			return Instance::new_int(lhs % get<IntT>(rhs.data));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(std::fmod(static_cast<FloatT>(lhs), get<FloatT>(rhs.data)));
		}
		throw MSG_BAD_CALL;
	}

	Instance operator()(const FloatT lhs) {
		if (holds<IntT>(rhs.data)) {
			return Instance::new_float(std::fmod(lhs, static_cast<FloatT>(get<IntT>(rhs.data))));
		} else if (holds<FloatT>(rhs.data)) {
			return Instance::new_float(std::fmod(lhs, get<FloatT>(rhs.data)));
		}
		throw MSG_BAD_CALL;
	}

	// str % str strips lhs of trailing chars in rhs
	Instance operator()(const StringT lhs) {
		if (!holds<StringT>(rhs.data)) {
			throw MSG_BAD_CALL;
		}
		StringT new_str = lhs;
		StringT rem		= get<StringT>(rhs.data);

		while (!new_str.empty()) {
			bool did_del = false;
			for (char c: rem) {
				if (new_str.at(new_str.size() - 1) == c) {
					new_str.erase(new_str.size() - 1);
					did_del = true;
				}
			}
			if (!did_del) {
				break;
			}
		}
		return Instance::new_string(new_str);
	}

	Instance operator()(const ArrayData lhs) { throw MSG_BAD_CALL; }

	Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; }
};
