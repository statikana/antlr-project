// string.h
#pragma once
#include "../kind.h"
#include "../object.h"
#include <string>

class String : public Object {
	private:
	std::string value;

	public:
	Kind kind	  = TYPE_KIND;
	TypeKind type = STRING_TYPE;

	String(const std::string& val) : value(val) {}

	std::string get_value() const { return value; }

	std::string get_text() const override { return "\"" + value + "\""; }
	bool get_truth() const override { return !value.empty(); }
};