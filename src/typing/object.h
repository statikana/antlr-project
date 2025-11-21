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

	virtual std::string get_text() const { return "object"; }
	virtual bool get_truth() const { return true; }
};
