#pragma once
#include <string>

#include "../kind.h"
#include "../object.h"

class Void : public Object {
	public:
	Kind kind	  = TYPE_KIND;
	TypeKind type = VOID_TYPE;
	Void() {}
	std::string get_text() const override { return "void"; }
	bool get_truth() const override { return false; }
};