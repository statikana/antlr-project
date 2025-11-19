#pragma once
#include "./kind.h"
#include "./object.h"
#include "./std/bool.h"
#include "./std/func.h"
#include "./std/int64.h"
#include "./std/void.h"

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
	TypeOrderNode* add(const TypeKind& subtype) {
		auto created = new TypeOrderNode(subtype);
		this->subtypes.push_back(created);
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

	std::string get_text(int indent=0) {
		string out = "";
		string prefix = "";
		std::sort(
			subtypes.begin(), subtypes.end(), 
			[](const TypeOrderNode* a, const TypeOrderNode* b){return a->subtypes.size() < b->subtypes.size(); });
		for (int i = 0; i < indent; i++) {
			prefix.append(" ");
		}
		out.append(type_kind_to_string[this->type] + "\n");
		for (auto child : subtypes) {
			out.append(prefix + "  |-" + child->get_text(indent+4) + "\n");
		}
		out = out.substr(0, out.find_last_not_of("\n")+1);
		return out;
	}
};

// creates the type order tree
TypeOrderNode create_tree() {
	auto root = TypeOrderNode(OBJECT_TYPE);
	root.add(INT64_TYPE)->add(BOOL_TYPE);
	root.add(VOID_TYPE);
	std::cout << root.get_text() << std::endl;
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

TypeOrderNode type_tree_root = create_tree();