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
	Kind kind	  = TYPE_KIND;
	TypeKind type = OBJECT_TYPE;

	virtual std::string to_text() const { return "object"; }
};
