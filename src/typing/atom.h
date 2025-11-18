#pragma once
#include "./object.h"
#include "./kind.h"
#include "./std/bool.h"
#include "./std/int64.h"
#include "./std/void.h"
#include "./std/func.h"

#include <utility>
#include <variant>
#include <vector>

using std::shared_ptr;
using std::string;
using std::vector;
using std::visit;


using Atom = std::variant<shared_ptr<Object>, shared_ptr<Bool>, shared_ptr<Void>, shared_ptr<Int64>, shared_ptr<Func>>;

// basic tree structure for resolving
// inheritance order. each node has a type
// and a list of children, each their own node.
struct TypeOrderNode {
	public:
	TypeKind type;
	vector<TypeOrderNode*> subtypes;

	TypeOrderNode(const TypeKind& t) : type(t), subtypes(vector<TypeOrderNode*>()) {}
	TypeOrderNode(const TypeKind& t, vector<TypeOrderNode*> subs) : type(t), subtypes(subs) {}

	// returns whether `type` is this node or any of its children
	bool contains(const TypeKind& t) const {
		if (type == t) {
			return true;
		}

		for (auto subtype: subtypes) {
			if (subtype->contains(t)) {
				return true;
			}
		}

		return false;
	}

	// adds a subtype to this node's subtypes, with no subtypes
	// of its own. Returns the created node.
	TypeOrderNode add(const TypeKind& subtype) {
		auto created = TypeOrderNode(subtype);
		this->subtypes.push_back(&created);
		return created;
	}

	// tries to find `instance`'s node in the tree.
	// returns nullptr if not found
	const TypeOrderNode* find_node(const TypeKind& t) const {
		if (this->type == t) {
			return this;
		}

		for (auto subtype: subtypes) {
			auto child_node = subtype->find_node(t);
			if (child_node != nullptr) {
				return child_node;
			}
		}

		return nullptr;
	}
};

// creates the type order tree
TypeOrderNode create_tree() {
	auto root = TypeOrderNode(OBJECT_TYPE);
	root.add(INT64_TYPE).add(BOOL_TYPE);
	root.add(VOID_TYPE);
	return root;
}


// wraps lambda visitors for getting attributes
// of every member of inner Atom
class AtomAccess {
    public:
    const Atom& inner;
    AtomAccess(const Atom& a) : inner(a) {}

    TypeKind get_type() {
	    return visit([](auto arg) { return arg->type; }, this->inner);
    }

    Kind get_kind() {
	    return visit([](auto arg) { return arg->kind; }, this->inner);
    }

    string get_text() {
        return visit([](auto arg) { return arg->to_text(); }, this->inner);
    }
};


bool subtype(const TypeOrderNode& root, Atom& parent, const Atom& child) {
	auto parent_acc = AtomAccess(parent);
	auto child_acc  = AtomAccess(child);

	const TypeOrderNode* parent_node = root.find_node(parent_acc.get_type());

	// parent defined in tree?
	if (parent_node == nullptr) {
		return false;
	}

	return parent_node->contains(child_acc.get_type());
}