#pragma once
#include "./atom.h"
#include "../ast/ast_node.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::shared_ptr;

class Scope;

typedef pair<shared_ptr<Scope>, shared_ptr<FuncDefNode>> FuncData;
typedef std::unordered_map<string, AtomV> Map;
typedef std::unordered_map<string, FuncData> FuncStorage;

class Scope {
	Map map;
	FuncStorage funcs;
	Scope* parent;
	Scope* child; // for debugging

	public:
	Scope() : map(), funcs(), parent(nullptr), child(nullptr) {}
	Scope(Scope* p) : map(), funcs(), parent(p) {
		p->child = this;
		child = nullptr;
	}

	Scope* get_parent() const { return parent; }
	Scope* get_child() const { return child; }

	void define(string name, AtomV value) { map.insert_or_assign(name, value); }

	AtomV get(string name) {
		if (map.count(name) != 0) {
			return map[name];
		}
		if (parent != nullptr) {
			return parent->get(name);
		}
		std::cerr << name + " is undefined.";
		throw std::runtime_error(name + " is undefined.");
	}

	bool is_defined(string name) const {
		if (map.count(name) != 0) {
			return true;
		}

		if (parent == nullptr) {
			return false;
		} else {
			return parent->is_defined(name);
		}
	}

	vector<string> names() const {
		vector<string> vec = vector<string>();
		for (auto pair: map) {
			vec.push_back(pair.first);
		}
		if (parent != nullptr) {
			auto pnames = parent->names();
			vec.insert(vec.end(), pnames.begin(), pnames.end());
		}
		return vec;
	}

	Scope* enter() {
		Scope* child = new Scope(this);
		this->child = child;
		return child;
	}

	bool is_func_defined(const string& name) const {
		if (funcs.count(name) != 0) {
			return true;
		}

		if (parent == nullptr) {
			return false;
		} else {
			return parent->is_func_defined(name);
		}
	}

	FuncData get_func(const string& name) {
		if (funcs.count(name) != 0) {
			return funcs.at(name);
		}
		if (parent != nullptr) {
			return parent->get_func(name);
		}
		std::cerr << name + " function is undefined.";
		throw std::runtime_error(name + " function is undefined.");
	}

	void define_func(const string& name, FuncData func_def) {
		funcs.insert_or_assign(name, func_def);
	}

	void print() {
		std::cout << "Scope [Vars: {";
		// print current level entries
		for (auto& pair: map) {
			std::cout << pair.first << ": " << AtomVAccess(pair.second).get_text() << ", ";
		}
		std::cout << "}";

		std::cout << " Funcs: {";
		for (auto& pair: funcs) {
			std::cout << pair.first << ": [";
			for (const auto& param: pair.second.second->get_params()) {
				std::cout << type_kind_to_string[param.first] << ",";
			}
			std::cout << " -> " << type_kind_to_string[pair.second.second->get_function_return_type()];
			std::cout << "], ";
		}
		std::cout << "}]";

		// optionally print parent scope
		if (parent != nullptr) {
			std::cout << " parent -> ";
			parent->print();
		} else {
			std::cout << " parent -> null";
		}
	}
};