#pragma once
#include <string>

#include "../kind.h"
#include "../object.h"

class Bool : public Object {
	private:
	bool state;

	public:
	Kind kind	  = TYPE_KIND;
	TypeKind type = BOOL_TYPE;

	Bool(bool value) : state(value) {}
	bool get_state() const { return state; }

	std::string get_text() const override { return state ? "true" : "false"; }
	bool get_truth() const override { return state; }
};