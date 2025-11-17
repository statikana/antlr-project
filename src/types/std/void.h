#pragma once
#include "../object.h"
#include "./bool.h"
#include <variant>

// void type. use VOID singleton instance to refer to void value
class Void : Object {
	public:
	Void() {}
	std::string to_text() const override { return "void"; }

	Object op_eq(const Object& other) const override {
		try {
			const Void& v = dynamic_cast<const Void&>(other);
			return Bool(true);
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}

	Object op_req(const Object& other) const override {
		try {
			const Void& v = dynamic_cast<const Void&>(other);
			return Bool(false);
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}
};

Void VOID = Void();