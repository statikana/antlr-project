#pragma once
#include "../object.h"
#include <variant>

// simple boolean type
class Bool : public Object {
	private:
	bool state;

	public:
	Bool(bool value) : state(value) {}
	bool getState() const { return state; }

	std::string to_text() const override { return state ? "true" : "false"; }

	Object op_eq(const Object& other) const override {
		try {
			const Bool& b = dynamic_cast<const Bool&>(other);
			return Bool(state == b.getState());
		} catch (const std::bad_cast) {
		}
		throw not_implemented;
	}
};