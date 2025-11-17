#include "../types/object.h"
#include "../types/std/bool.h"
#include "../types/std/int64.h"
#include "../types/std/void.h"
#include <variant>

// Internal function to perform operation
// if the operation fails, it attempts the reversed operation (i.e. tries lhs.op_add(rhs), then rhs.op_radd(lhs))
Object operate(const Object& lhs, const std::string& op, const Object& rhs) {
	try {
		return _operate(lhs, op, rhs, false);
	} catch (const NotImplementedT&) {
		try {
			return _operate(rhs, op, lhs, true);
		} catch (const NotImplementedT&) {
			throw not_implemented;
		}
	}
}

// Helper function to call the appropriate operation method
// is_reversed indicates whether to call the reversed operation (i.e. op_radd instead of op_add)
Object _operate(const Object& lhs, const std::string& op, const Object& rhs, bool is_reversed) {
	// order of arguments is not different if is_reversed is true
	// lhs and rhs should already be swapped by caller
	if (op == "+") {
		return is_reversed ? lhs.op_radd(rhs) : lhs.op_add(rhs);
	} else if (op == "-") {
		return is_reversed ? lhs.op_rsub(rhs) : lhs.op_sub(rhs);
	} else if (op == "*") {
		return is_reversed ? lhs.op_rmul(rhs) : lhs.op_mul(rhs);
	} else if (op == "/") {
		return is_reversed ? lhs.op_rdiv(rhs) : lhs.op_div(rhs);
	} else if (op == "%") {
		return is_reversed ? lhs.op_rmod(rhs) : lhs.op_mod(rhs);
	} else if (op == "\\") {
		return is_reversed ? lhs.op_rfld(rhs) : lhs.op_fld(rhs);
	} else if (op == "**") {
		return is_reversed ? lhs.op_rpow(rhs) : lhs.op_pow(rhs);
	} else if (op == "==") {
		return is_reversed ? lhs.op_req(rhs) : lhs.op_eq(rhs);
	} else {
		throw not_implemented;
	}
}