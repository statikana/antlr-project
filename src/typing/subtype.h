#include "./atom.h"
#include "./not_implemented.h"

using std::get;
using std::holds_alternative;

using std::shared_ptr;

// uses the root tree order node to determine if child is a subtype of parent
bool _type_subtype(const TypeKind& parent_t, const TypeKind& child_t) {
	const TypeOrderNode* parent_node = type_tree_root.find_node(parent_t);
	if (parent_node == nullptr) {
		return false;
	}
	return parent_node->contains(child_t);
}

bool subtype(const Atom& parent, const Atom& child) {
	auto pa = AtomAccess(parent);
	auto ca = AtomAccess(child);

	auto parent_kind = pa.get_kind();
	auto child_kind	 = ca.get_kind();

	if (parent_kind != child_kind) {
		return false;
	}

	switch (parent_kind) {
        case TYPE_KIND: {
            // use type tree
            return _type_subtype(pa.get_type(), ca.get_type());
        }
        case FUNCTION_KIND: {
            if (!holds_alternative<shared_ptr<Func>>(parent) or !holds_alternative<shared_ptr<Func>>(child)) {
                throw std::runtime_error("Type marked as FUNCTION_KIND is not of type Func");
            }
            auto parent_f = get<shared_ptr<Func>>(parent);
            auto child_f  = get<shared_ptr<Func>>(child);

            auto parent_param_ts = parent_f->get_param_ts();
            auto child_param_ts	 = child_f->get_param_ts();

            auto parent_param_names = parent_f->get_param_names();
            auto child_param_names	= child_f->get_param_names();

            // ensure same # parameters
            if (parent_param_ts.size() != child_param_ts.size()) {
                return false;
            }

            // covariance: ensure that the child's return type is smaller, or equal to, the parent's
            if (!_type_subtype(parent_f->get_return_t(), child_f->get_return_t())) {
                return false;
            }

            for (int i = 0; i < parent_param_ts.size(); i++) {
                // ensure same param name
                if (parent_param_names[i] != child_param_names[i]) {
                    return false;
                }

                // contravariance: ensure child's parameter type is at least as broad as the parent's
                if (!_type_subtype(child_param_ts[i], parent_param_ts[i])) {
                    return false;
                }
            }
            return true;
        }
        default:
            return false;
	}
}
