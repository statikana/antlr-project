#include "../order.h"

bool equal(const Atom& left, const Atom& right) {
	TypeKind left_t	 = get_type(left);
	TypeKind right_t = get_type(right);

	if (left_t != right_t) {
		return false;
	}

	// TODO

	return false;
}