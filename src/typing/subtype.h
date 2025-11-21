#pragma once
#include "./atom.h"
#include "./not_implemented.h"

using std::get;
using std::holds_alternative;

using std::shared_ptr;

// uses the root tree order node to determine if child is a subtype of parent
bool type_subtype(const TypeKind& parent_t, const TypeKind& child_t) {
	const TypeOrderNode* parent_node = type_tree_root.find_node(parent_t);
	if (parent_node == nullptr) {
		return false;
	}
	return parent_node->contains(child_t);
}

bool subtype(const AtomV& parent, const AtomV& child) {
	auto pa = AtomVAccess(parent);
	auto ca = AtomVAccess(child);

	auto parent_kind = pa.get_kind();
	auto child_kind	 = ca.get_kind();

	if (parent_kind != child_kind) {
		return false;
	}

	switch (parent_kind) {
	case TYPE_KIND: {
		// use type tree
		return type_subtype(pa.get_type(), ca.get_type());
	}
	case FUNCTION_KIND: {
		if (!holds_alternative<shared_ptr<Func>>(parent) or !holds_alternative<shared_ptr<Func>>(child)) {
			throw std::runtime_error("Type marked as FUNCTION_KIND is not of type Func");
		}
		auto parent_f = get<shared_ptr<Func>>(parent);
		auto child_f  = get<shared_ptr<Func>>(child);

		auto parent_params = parent_f->get_params();
		auto child_params = child_f->get_params();

		// ensure same # parameters
		if (parent_params.size() != child_params.size()) {
			return false;
		}

		// covariance: ensure that the child's return type is smaller, or equal to, the parent's
		if (!type_subtype(parent_f->get_return_type(), child_f->get_return_type())) {
			return false;
		}

		for (int i = 0; i < parent_params.size(); i++) {
			// ensure same param name
			if (parent_params[i].second != child_params[i].second) {
				return false;
			}

			// contravariance: ensure child's parameter type is at least as broad as the parent's
			if (!type_subtype(child_params[i].first, parent_params[i].first)) {
				return false;
			}
		}
		return true;
	}
	default:
		return false;
	}
}
