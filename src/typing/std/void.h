#pragma once
#include "../order.h"
#include <variant>

class Void : public Object {
	public:
	TypeKind kind = VOID;
	Void() {}
	std::string to_text() const override { return "void"; }
};