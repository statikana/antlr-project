// float.h

#pragma once
#include "../kind.h"
#include "../object.h"
#include <string>

class Float : public Object {
	private:
	double value;

	public:
	Kind kind	  = TYPE_KIND;
	TypeKind type = FLOAT_TYPE;

	Float(double v) : value(v) {}

	double get_value() const { return value; }

	std::string get_text() const override {
		auto text = std::to_string(value);
		text	  = text.substr(0, text.find_last_not_of('0') + 1);
		if (text.back() == '.') {
			text += "0";
		}
		return text;
	}
	bool get_truth() const override { return value != 0; }
};