#pragma once
#include <string>

#include "../object.h"
#include "../kind.h"

class Void : public Object {
	public:
	TypeKind kind = VOID;
	Void() {}
	std::string to_text() const override { return "void"; }
};