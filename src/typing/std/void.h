#pragma once
#include <string>

#include "../object.h"
#include "../kind.h"

class Void : public Object {
	public:
	Kind kind = TYPE_KIND;
	TypeKind type = VOID_TYPE;
	Void() {}
	std::string to_text() const override { return "void"; }
};