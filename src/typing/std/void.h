#pragma once
#include <string>

#include "../kind.h"
#include "../object.h"

class Void : public Object {
	public:
	Kind kind	  = TYPE_KIND;
	TypeKind type = VOID_TYPE;
	Void() {}
	std::string to_text() const override { return "void"; }
};