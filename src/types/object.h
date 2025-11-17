#pragma once
#include <memory>
#include <stdexcept>
#include <string>

class NotImplementedT : public std::runtime_error {
	std::string what_message = "Operation not implemented";

	public:
	NotImplementedT() : std::runtime_error(what_message) {}

	const char* what() const noexcept override { return what_message.c_str(); }
};

NotImplementedT not_implemented = NotImplementedT();

// Base class for all other objects in the type system
class Object {
	public:
	virtual Object op_add(const Object& other) const { throw not_implemented; } // +
	virtual Object op_sub(const Object& other) const { throw not_implemented; } // -
	virtual Object op_mul(const Object& other) const { throw not_implemented; } // *
	virtual Object op_div(const Object& other) const { throw not_implemented; } // /
	virtual Object op_mod(const Object& other) const { throw not_implemented; } // %
	virtual Object op_fld(const Object& other) const { throw not_implemented; } // <backslash>
	virtual Object op_pow(const Object& other) const { throw not_implemented; } // **

	// reverse methods (called if other.op_xxx(this) is not implemented)
	virtual Object op_radd(const Object& other) const { throw not_implemented; }
	virtual Object op_rsub(const Object& other) const { throw not_implemented; }
	virtual Object op_rmul(const Object& other) const { throw not_implemented; }
	virtual Object op_rdiv(const Object& other) const { throw not_implemented; }
	virtual Object op_rmod(const Object& other) const { throw not_implemented; }
	virtual Object op_rfld(const Object& other) const { throw not_implemented; }
	virtual Object op_rpow(const Object& other) const { throw not_implemented; }

	virtual Object op_eq(const Object& other) const { throw not_implemented; }
	virtual Object op_req(const Object& other) const { throw not_implemented; }

	virtual std::string to_text() const { return "object"; }
};