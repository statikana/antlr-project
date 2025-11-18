#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>

#include "./kind.h"

// base object in type systems. all others should
// inherit from this
class Object {
	public:
	Brand brand	  = TYPE;
	TypeKind kind = OBJECT;

	virtual std::string to_text() const { return "object"; }
};
