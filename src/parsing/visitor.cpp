#pragma once
#include "antlr4-runtime.h"

#include "../../gen_cpp/KodaBaseVisitor.h"
#include "../../gen_cpp/KodaParser.h"

#include "../typing/scope.cpp"
#include "../typing/atom.h"
#include "../typing/subtype.h"


using namespace antlr4;

using std::any;
using std::any_cast;
using std::get;
using std::make_shared;
using std::move;
using std::shared_ptr;
using std::string;

typedef antlr4::tree::ParseTree Node;

class MainKodaVisitor : public KodaBaseVisitor {
	private:
	Scope scope = Scope();
	
	template<typename T> any wrap(const T& obj) { return any(obj); }

	Atom unwrap(const any& v) { return any_cast<Atom>(v); }

	public:
	//
	// statement rules
	//
	any visitFunctionDefinition(KodaParser::FunctionDefinitionContext* ctx) override {
		string func_name = ctx->func_name->getText();
		auto params		 = ctx->params->children;

		vector<TypeKind> param_ts;
		vector<string> param_names;

		Node* last = nullptr;
		for (Node* param: params) {
			string param_txt = param->getText();
			if (param_txt == "," | param_txt == "(" | param_txt == ")") {
				continue;
			}
			if (last == nullptr) {
				last = param;
				continue;
			}
			std::cout << last->getText() << " " << param->getText() << std::endl;
			TypeKind param_t  = string_to_type_kind[last->getText()];
			string param_name = param->getText();

			param_ts.push_back(param_t);
			param_names.push_back(param_name);
		}

		TypeKind return_t = string_to_type_kind[ctx->ret_t->getText()];
		const Func& func  = Func(param_ts, param_names, return_t);
		const Atom& atom  = make_shared<Func>(move(func));
		scope.define(func_name, atom);
		std::cout << scope.get_text() << std::endl;

		if (scope.is_defined("foo_jr")){
		std::cout << "IS SUBTYPE: " << subtype(scope["foo"], scope["foo_jr"]) << std::endl;}
		return nullptr;
	}

	any visitVariableDefinition(KodaParser::VariableDefinitionContext* ctx) override {
		// don't actually do anything with the variable type?
		string type = ctx->type->getText();
		string id  = ctx->name->getText();
		Atom value = unwrap(visit(ctx->expression()));

		scope.define(id, value);
		std::cout << scope.get_text() << std::endl;
		return nullptr;
	}

	//
	// expression rules
	//
	any visitArithmeticExpr(KodaParser::ArithmeticExprContext* ctx) override {
		Atom lhs = unwrap(visit(ctx->lhs));
		Atom rhs = unwrap(visit(ctx->rhs));

		// TODO: math inference
		// return wrap(Int64(42));
		return nullptr;
	}

	any visitFunctionCall(KodaParser::FunctionCallContext* ctx) override {
		string func_name = ctx->func_name->getText();
		auto* args		 = ctx->args;

		// TODO: arg unwrapping
		// TODO: typing
		return nullptr;
	}

	// blocks can be created by the user whenever, but must be
	// created when entering a definition that is not a variable (function, class)
	any visitBlock(KodaParser::BlockContext* ctx) override {
		// this gets called when a function gets created, and the scope is scored
		// with the function definition.

	}
};