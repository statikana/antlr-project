#pragma once

#include "./atom.h"
#include "./kind.h"

#include <utility>
#include <variant>
#include <vector>

using std::shared_ptr;
using std::string;
using std::vector;
using std::visit;

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
	auto root = TypeOrderNode(OBJECT);
	root.add(INT64).add(BOOL);
	root.add(VOID);
	return root;
}

TypeKind get_type(const Atom& object) {
	return visit([](auto arg) { return arg->kind; }, object);
}

string get_to_text(const Atom& object) {
	return visit([](auto arg) { return arg->to_text(); }, object);
}

bool subtype(const TypeOrderNode& root, Atom& parent, const Atom& child) {
	TypeKind parent_t = get_type(parent);
	TypeKind child_t  = get_type(child);

	const TypeOrderNode* parent_node = root.find_node(parent_t);

	// parent defined in tree?
	if (parent_node == nullptr) {
		return false;
	}

	return parent_node->contains(child_t);
}