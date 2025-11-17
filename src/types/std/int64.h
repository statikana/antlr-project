#pragma once
#include "../object.h"
#include "./bool.h"
#include "./void.h"
#include <cmath>

// 64-bit signed integer type
class Int64 : public Object {
	private:
	int64_t value;

	public:
	Int64(int64_t value) : value(value) {}
	int64_t getValue() const { return value; }

	std::string to_text() const override { return std::to_string(value); }

	Object op_eq(const Object& other) const override {
		try {
			const Int64& i = dynamic_cast<const Int64&>(other);
			return Bool(value == i.getValue());
		} catch (const std::bad_cast) {
		}
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Bool(value == static_cast<int64_t>(b.getState()));
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_add(const Object& other) const override {
		try {
			const Int64& i = dynamic_cast<const Int64&>(other);
			return Int64(value + i.getValue());
		} catch (const std::bad_cast) {
		}
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(value + static_cast<int64_t>(b.getState()));
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_sub(const Object& other) const override {
		try {
			const Int64& i = dynamic_cast<const Int64&>(other);
			return Int64(value - i.getValue());
		} catch (const std::bad_cast) {
		}
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(value - static_cast<int64_t>(b.getState()));
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_mul(const Object& other) const override {
		try {
			const Int64& i = dynamic_cast<const Int64&>(other);
			return Int64(value * i.getValue());
		} catch (const std::bad_cast) {
		}
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(value * static_cast<int64_t>(b.getState()));
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_div(const Object& other) const override {
		try {
			const Int64& i = dynamic_cast<const Int64&>(other);
			return Int64(value / i.getValue());
		} catch (const std::bad_cast) {
		}
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(value / static_cast<int64_t>(b.getState()));
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_mod(const Object& other) const override {
		try {
			const Int64& i = dynamic_cast<const Int64&>(other);
			return Int64(value % i.getValue());
		} catch (const std::bad_cast) {
		}
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(value % static_cast<int64_t>(b.getState()));
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_fld(const Object& other) const override {
		try {
			const Int64& i = dynamic_cast<const Int64&>(other);
			return Int64(value / i.getValue());
		} catch (const std::bad_cast) {
		}
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(value / static_cast<int64_t>(b.getState()));
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_pow(const Object& other) const override {
		try {
			const Int64& i = dynamic_cast<const Int64&>(other);
			return Int64(static_cast<int64_t>(value, i.getValue()));
		} catch (const std::bad_cast) {
		}
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(b.getState() ? Int64(value) : Int64(1));
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_radd(const Object& other) const override {
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(static_cast<int64_t>(b.getState()) + value);
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_rsub(const Object& other) const override {
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(static_cast<int64_t>(b.getState()) - value);
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_rmul(const Object& other) const override {
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(static_cast<int64_t>(b.getState()) * value);
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_rdiv(const Object& other) const override {
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(static_cast<int64_t>(b.getState()) / value);
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_rmod(const Object& other) const override {
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(static_cast<int64_t>(b.getState()) % value);
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_rfld(const Object& other) const override {
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Int64(static_cast<int64_t>(b.getState()) / value);
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_rpow(const Object& other) const override {
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			// 1^n = 1
			// 0^0 = 1
			// 0^n = 0 for n > 0X
			return Int64(b.getState() ? Int64(1) : (value == 0 ? Int64(1) : Int64(0)));
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}
};