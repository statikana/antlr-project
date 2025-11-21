// represents an operation or value
#pragma once
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "../typing/std/bool.h"
#include "../typing/std/float.h"
#include "../typing/std/int64.h"
#include "../typing/std/string.h"
#include "../typing/std/void.h"
#include "../typing/std/func.h"

#include "../typing/kind.h"

using std::map;
using std::pair;
using std::shared_ptr;
using std::string;
using std::variant;
using std::vector;

string INDENT = "    ";

string rpt(int n) {
	string out = "";
	for (int i = 0; i < n; i++) { out.append(INDENT); }
	return out;
}

class Node {
	public:
	virtual ~Node() = default;
	virtual string get_text(int indent = 0) const { return rpt(indent) + "Node"; }
};

class BodyNode : public Node {
	private:
	vector<shared_ptr<Node>> nodes;

	public:
	BodyNode(const vector<shared_ptr<Node>>& n) : nodes(n) {}
	string get_text(int indent = 0) const override {
		string out = rpt(indent) + "Body:\n";
		for (auto node: nodes) { out += node->get_text(indent + 1) + "\n"; }
		return out;
	}
	auto const& getNodes() const noexcept { return nodes; }
};

class VarDefNode : public Node {
	TypeKind type;
	string name;

	public:
	VarDefNode(const TypeKind& t, const string& n) : type(t), name(n) {}
	string get_text(int indent = 0) const override {
		return rpt(indent) + "Var Def: " + type_kind_to_string[type] + " " + name;
	}
	auto get_type() const { return type; }
	auto get_name() const { return name; }
};

class VarAssignNode : public Node {
	string name;
	shared_ptr<Node> value;

	public:
	VarAssignNode(const string& n, shared_ptr<Node> v) : name(n), value(v) {}
	string get_text(int indent = 0) const override {
		return rpt(indent) + "Var Assign: " + name + " = [" + value->get_text(0) + "]";
	}
	auto get_name() const { return name; }
	auto get_value() const { return value; }
};

class VarDefAndAssignNode : public Node {
	TypeKind type;
	string name;
	shared_ptr<Node> value;

	public:
	VarDefAndAssignNode(const TypeKind& t, const string& n, shared_ptr<Node> v) : type(t), name(n), value(v) {}
	string get_text(int indent = 0) const override {
		return rpt(indent) + "Var Def and Assign: " + type_kind_to_string[type] + " " + name + " = [" +
			value->get_text(0) + "]";
	}
	auto get_type() const { return type; }
	auto get_name() const { return name; }
	auto get_value() const { return value; }
};

class FuncDefNode : public Node {
	string name;
	Func func;
	shared_ptr<BodyNode> body;

	public:
	FuncDefNode(const string& n, const vector<pair<TypeKind, string>>& p, const TypeKind& r, shared_ptr<BodyNode> b)
		: name(n), func(p, r), body(b) {}

	string get_name() const { return name; }
	vector<pair<TypeKind, string>> get_params() const { return func.get_params(); }
	TypeKind get_function_return_type() const { return func.get_return_type(); }
	string get_text(int indent = 0) const override {
		string out = rpt(indent) + "Func Def: " + name + "\n";
		out += rpt(indent + 1) + "Parameters:\n";
		for (const auto& param: func.get_params()) {
			out += rpt(indent + 2) + type_kind_to_string[param.first] + " " + param.second + "\n";
		}
		out += rpt(indent + 1) + "Return Type: " + type_kind_to_string[func.get_return_type()] + "\n";
		out += rpt(indent + 1) + "Func Body:\n" + body->get_text(indent + 2) + "\n";
		return out;
	}
	shared_ptr<BodyNode> get_body() const { return body; }
};

class FuncCallNode : public Node {
	string name;
	vector<shared_ptr<Node>> params;

	FuncCallNode(string n, vector<shared_ptr<Node>> p) : name(n), params(p) {}
	string get_text(int indent = 0) const override {
		string out = rpt(indent) + "Func Call: " + name + "\n";
		out += rpt(indent + 1) + "Parameters:\n";
		for (const auto& param: params) {
			out += param->get_text(indent + 2) + "\n";
		}
		return out;
	}

	auto const& get_name() const noexcept { return name; }
	auto const& get_params() const noexcept { return params; }
};

// BINARY ARITHMETIC NODE

enum BinArithOperator {
	ADD,
	SUB,
	MUL,
	DIV
};

map<string, BinArithOperator> string_to_binary_arith_operator = {{"+", ADD}, {"-", SUB}, {"*", MUL}, {"/", DIV}};

class BinArithNode : public Node {
	shared_ptr<Node> lhs;
	shared_ptr<Node> rhs;
	BinArithOperator op;

	public:
	BinArithNode(shared_ptr<Node> l, shared_ptr<Node> r, const BinArithOperator& o) : lhs(l), rhs(r), op(o) {}
	string get_text(int indent = 0) const override {
		string op_str;
		switch (op) {
		case ADD:
			op_str = "+";
			break;
		case SUB:
			op_str = "-";
			break;
		case MUL:
			op_str = "*";
			break;
		case DIV:
			op_str = "/";
			break;
		default:
			op_str = "?";
			break;
		}
		return rpt(indent) + "BinArith: [" + lhs->get_text(0) + "] " + op_str + " [" + rhs->get_text(0) + "]";
	}
	shared_ptr<Node> getLHS() const { return lhs; }
	shared_ptr<Node> getRHS() const { return rhs; }
	BinArithOperator getOp() const { return op; }
};

//
// Leafs (no internal nodes)
//

// BOOLEAN NODE
class BooleanNode : public Node {
	Bool value;

	public:
	BooleanNode(bool v) : value(v) {}
	string get_text(int indent = 0) const override {
		return rpt(indent) + "Boolean: " + string(value.get_state() ? "true" : "false");
	}
	Bool get_value() const { return value; }
};

// Int64 NODE
class Int64Node : public Node {
	Int64 value;

	public:
	Int64Node(int64_t v) : value(v) {}
	string get_text(int indent = 0) const override {
		return rpt(indent) + "Int64: " + std::to_string(value.get_value());
	}
	Int64 get_value() const { return value; }
};

class FloatNode : public Node {
	Float value;

	public:
	FloatNode(double v) : value(v) {}
	string get_text(int indent = 0) const override { return rpt(indent) + "Float: " + value.get_text(); }
	Float get_value() const { return value; }
};

// Void NODE
class VoidNode : public Node {
	public:
	VoidNode() {}
	string get_text(int indent = 0) const override { return rpt(indent) + "Void"; }
};

class IDNode : public Node {
	string name;

	public:
	IDNode(const string& n) : name(n) {}
	string get_text(int indent = 0) const override { return rpt(indent) + "ID: " + name; }

	auto const& get_name() noexcept { return name; }
	auto get_name() const { return name; }
};

class StringNode : public Node {
	String value;

	public:
	StringNode(const string& val) : value(val) {}
	string get_text(int indent = 0) const override { return rpt(indent) + "String: \"" + value.get_value() + "\""; }
	auto const& get_value() const noexcept { return value; }
};

class IfElseNode : public Node {
	shared_ptr<Node> condition;
	shared_ptr<BodyNode> if_body;
	shared_ptr<BodyNode> else_body;

	public:
	IfElseNode(shared_ptr<Node> c, shared_ptr<BodyNode> i, shared_ptr<BodyNode> e)
		: condition(c), if_body(i), else_body(e) {}
	IfElseNode(shared_ptr<Node> c, shared_ptr<BodyNode> i) : condition(c), if_body(i), else_body(nullptr) {}
	string get_text(int indent = 0) const override {
		string out = rpt(indent) + "IfElse:\n";
		string cond_text = condition->get_text(0); // inline
		string if_body_text = if_body->get_text(indent + 1);

		out += rpt(indent + 1) + "Condition: [" + cond_text + "]\n";
		out += rpt(indent + 1) + "If Body:\n" + if_body_text + "\n";
		if (else_body != nullptr) {
			auto eb = else_body->get_text(indent + 1);
			out += rpt(indent + 1) + "Else Body:\n" + eb + "\n";
		} else {
			out += rpt(indent + 1) + "Else Body: [null]\n";
		}
		out = out.substr(0, out.find_last_not_of("\n") + 1); // trim trailing newline
		return out;
	}
	void addElseBody(shared_ptr<BodyNode> eb) { else_body = eb; }
	auto const& getCondition() const { return condition; }
	auto const& getIfBody() const { return if_body; }
	auto const& getElseBody() const { return else_body; }
};
