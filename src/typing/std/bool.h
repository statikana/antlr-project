#pragma once
#include "../order.h"
#include <variant>

// simple boolean type
class Bool : public Object {
	private:
	bool state;

	public:
	TypeKind kind = BOOL;

	Bool(bool value) : state(value) {}
	bool getState() const { return state; }

	std::string to_text() const override { return state ? "true" : "false"; }
};