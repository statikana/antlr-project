#pragma once
#include <map>
#include <string>
#include <vector>

#include "../kind.h"
#include "../object.h"

using std::map;
using std::string;
using std::vector;

class Func : public Object {
	private:
	// types of parameters (int, bool, float)
	vector<TypeKind> param_ts;
	// names of parameters
	vector<string> param_names;

	// return type
	TypeKind return_t;

	public:
	Kind kind	  = FUNCTION_KIND;
	TypeKind type = FUNCTION_TYPE;

	Func(vector<TypeKind> types, vector<string> names, TypeKind ret)
		: param_ts(types), param_names(names), return_t(ret) {
		for (int i = 0; i < param_ts.size(); i++) {
			std::cout << "  " << type_kind_to_string[param_ts[i]] << " " << param_names[i] << std::endl;
		}
	}

	vector<TypeKind> get_param_ts() { return param_ts; }
	vector<string> get_param_names() { return param_names; }
	TypeKind get_return_t() { return return_t; }

	std::string to_text() const override {
		std::string out = "(";

		for (int i = 0; i < param_ts.size() - 1; i++) {

			out.append(type_kind_to_string[param_ts[i]] + " " + param_names[i] + ", ");
		}
		out.append(type_kind_to_string[param_ts[param_ts.size() - 1]] + " " + param_names[param_names.size() - 1]);

		out += ") -> " + type_kind_to_string[return_t];
		return out;
	}
};