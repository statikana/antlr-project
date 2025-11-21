#pragma once
#include <string>

#include "../kind.h"
#include "../object.h"

// 64-bit signed integer type
class Int64 : public Object {
	private:
	int64_t value;

	public:
	Kind kind	  = TYPE_KIND;
	TypeKind type = INT64_TYPE;

	Int64(int64_t value) : value(value) {}
	int64_t get_value() const { return value; }

	std::string get_text() const override { return std::to_string(value); }
	bool get_truth() const override { return value != 0; }
};
