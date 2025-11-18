#pragma once
#include "./typing/atom.h"
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

	public:
	Scope() {}

	Atom& operator[](std::string name) {
		if (!is_defined(name)) {
			throw name + " not defined. Use .define";
		}
		return map.at(name);
	}

	void define(std::string name, const Atom& value) { map.insert_or_assign(name, value); }

	bool is_defined(std::string name) { return map.count(name) != 0; }

	std::vector<std::string> names() {
		std::vector<std::string> names_vec;
		for (auto const& val: map) {
			names_vec.push_back(val.first);
		}
		return names_vec;
	}

	void print() {
		std::cout << "Current Scope:" << std::endl;
		int counter = 1;
		for (const auto& vals: map) {
			auto access = AtomAccess(vals.second);
			std::cout << "Defintion " << counter << ":" << std::endl;
			std::cout << "  Name:  '" << vals.first << "'" << std::endl;
			std::cout << "  Text:  '" << access.get_text() << "'" << std::endl;
			std::cout << "  Kind:   " << kind_to_string[access.get_kind()] << std::endl;
			std::cout << "  Type:   " << type_kind_to_string[access.get_type()] << std::endl;
			std::cout << std::endl << std::endl;
			counter++;
		}
	}
};