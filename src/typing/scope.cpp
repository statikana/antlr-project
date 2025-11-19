#pragma once
#include "./atom.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, Atom> Map;

// represents a scope for variable definitions
// in the future, will expand to support nested scopes
// currently basically a wrapper around an unordered_map
class Scope {
	private:
	Map map;
	Scope* parent = nullptr;

	public:
	Scope() {}
	Scope(Scope* p) : parent(p) {}

	Atom& operator[](std::string name) {
		if (!is_defined(name)) {
			throw name + " not defined. Use .define";
		}
		return get_map().at(name);
	}

	void define(std::string name, const Atom& value) { map.insert_or_assign(name, value); }

	// gets if `name` is a valid ID in this scope, including parents.
	bool is_defined(std::string name) {
		if (is_defined_here(name)) { return true; }
		return parent == nullptr ? false : parent->is_defined(name); 
	}

	// gets if `name` is a valid ID in this scope, excluding parents.
	bool is_defined_here(std::string name) {
		return map.count(name) != 0;
	}

	std::vector<std::string> names() {
		std::vector<std::string> names_vec;
		for (const auto& val: get_map()) {
			names_vec.push_back(val.first);
		}
		return names_vec;
	}
	
	Map get_map() {
		Map current = map;
		if (parent != nullptr) {
			current.merge(parent->get_map());
		}
		return current;
	}

	string get_text() {
		string out = "";
		out += "Current Scope:\n";
		int counter = 1;
		for (const auto& vals: get_map()) {
			auto access = AtomAccess(vals.second);
			out += "Defintion " + std::to_string(counter) + ":";
			out += "  Name:  '" + vals.first + "'\n";
			out += "  Text:  '" + access.get_text() + "'\n";
			out += "  Kind:   " + kind_to_string[access.get_kind()] + "\n";
			out += "  Type:   " + type_kind_to_string[access.get_type()] + "\n";
			counter++;
		}
		return out;
	}

	// creates a new scope with `this` as it's parent, then returns it.
	Scope* enter() {
		Scope* child = new Scope();
		child->parent = this;
		return child;
	}
	
};