#pragma once
#include "./order.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>

#include "./not_implemented.h"

// base object in type systems. all others should
// inherit from this
class Object {
	public:
	Brand brand	  = TYPE;
	TypeKind kind = OBJECT;

	virtual std::string to_text() const { return "object"; }
};
