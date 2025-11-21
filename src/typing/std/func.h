#pragma once
#include "../object.h"

using std::pair;
using std::string;
using std::vector;

class Func : public Object {
	public:
	Kind kind	  = FUNCTION_KIND;
	TypeKind type = FUNCTION_TYPE;
	vector<pair<TypeKind, string>> params;
	TypeKind return_type;

	Func(const vector<pair<TypeKind, string>>& p, const TypeKind& r) : params(p), return_type(r) {}

	vector<pair<TypeKind, string>> get_params() const { return params; }
	TypeKind get_return_type() const { return return_type; }

	string get_text() const override {
		string out = "function(";
		for (size_t i = 0; i < params.size(); ++i) {
			out += type_kind_to_string[params[i].first] + " " + params[i].second;
			if (i < params.size() - 1) {
				out += ", ";
			}
		}
		out += ") -> " + type_kind_to_string[return_type];
		return out;
	}
	bool get_truth() const override { return false; }
};