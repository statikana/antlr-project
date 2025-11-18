#pragma once
#include "../order.h"
#include <cmath>
#include <memory>
#include <variant>

using std::get_if;
using std::make_shared;

// 64-bit signed integer type
class Int64 : public Object {
	private:
	int64_t value;

	public:
	TypeKind kind = INT64;
	Int64(int64_t value) : value(value) {}
	int64_t getValue() const { return value; }

	std::string to_text() const override { return std::to_string(value); }
};
