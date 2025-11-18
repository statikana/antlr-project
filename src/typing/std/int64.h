#pragma once
#include <string>

#include "../object.h"
#include "../kind.h"

// 64-bit signed integer type
class Int64 : public Object {
	private:
	int64_t value;

	public:
	Kind kind = TYPE_KIND;
	TypeKind type = INT64_TYPE;

	Int64(int64_t value) : value(value) {}
	int64_t getValue() const { return value; }

	std::string to_text() const override { return std::to_string(value); }
};
