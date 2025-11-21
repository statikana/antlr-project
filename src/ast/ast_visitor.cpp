#pragma once
#include "antlr4-runtime.h"

#include "../../gen_cpp/KodaBaseVisitor.h"
#include "../../gen_cpp/KodaParser.h"

#include "../typing/atom.h"
#include "../typing/scope.cpp"
#include "../typing/subtype.h"
#include "./ast_node.h"

#include <utility>
// visits the tree and creates AST nodes representing the syntax tree.

using std::any;
using std::any_cast;
using std::dynamic_pointer_cast;
using std::make_shared;
using std::move;
using std::pair;
using std::vector;

// PATTERN:
//
class AstVisitor : public KodaBaseVisitor {
	public:
	// gets an expression's value and converts it to a
	// shared_ptr to a Node
	shared_ptr<Node> fetch(antlr4::ParserRuleContext* ctx) {
		auto r = visit(ctx);
		return std::any_cast<shared_ptr<Node>>(r);
	}

	// gives the value to a shared pointer, then wraps
	// it in std::any
	template<typename TypeT, typename... ArgT> any wrap(const ArgT&... values) {
		// todo: apply std::move
		auto specific = make_shared<TypeT>(values...);
		shared_ptr<Node> base = specific;
		return any(base);
	}

	any visitProgram(KodaParser::ProgramContext* ctx) override {
		vector<shared_ptr<Node>> lines;
		for (auto statement: ctx->statement()) { lines.push_back(fetch(statement)); }
		// this is the
		return wrap<BodyNode>(lines);
	}

	any visitStatement(KodaParser::StatementContext* ctx) override { return visit(ctx->children[0]); }

	any visitBlock(KodaParser::BlockContext* ctx) {
		vector<shared_ptr<Node>> lines;
		for (auto s: ctx->statement()) { lines.push_back(fetch(s)); }
		return wrap<BodyNode>(lines);
	}

	any visitIfElseControlSeq(KodaParser::IfElseControlSeqContext* ctx) override {
		auto blocks = ctx->block();
		auto exprs = ctx->expression();
		auto first = blocks[0];
		auto* root = new IfElseNode(fetch(exprs[0]), dynamic_pointer_cast<BodyNode>(fetch(blocks[0])));

		if (ctx->last_e == nullptr) {
			// only ifs and elifs
			auto current = root;
			for (int i = 1; i < blocks.size(); i++) {
				auto* created = new IfElseNode(fetch(exprs[i]), dynamic_pointer_cast<BodyNode>(fetch(blocks[i])));
				shared_ptr<IfElseNode> created_shared(created);

				vector<shared_ptr<Node>> else_body;

				else_body.push_back(created_shared);
				auto body = make_shared<BodyNode>(else_body);
				current->addElseBody(body);
				current = created;
			}
		} else {
			if (blocks.size() == 2) {
				root->addElseBody(dynamic_pointer_cast<BodyNode>(fetch(blocks[2])));
			} else {
				auto current = root;
				for (int i = 1; i < blocks.size() - 1; i++) {
					auto* created = new IfElseNode(fetch(exprs[i]), dynamic_pointer_cast<BodyNode>(fetch(blocks[i])));
					shared_ptr<IfElseNode> created_shared(created);

					vector<shared_ptr<Node>> else_body;

					else_body.push_back(created_shared);
					auto body = make_shared<BodyNode>(else_body);
					current->addElseBody(body);
					current = created;
				}
				int last = blocks.size() - 1;
				current->addElseBody(dynamic_pointer_cast<BodyNode>(fetch(blocks[last])));
			}
		}
		// this is basically wrap
		shared_ptr<IfElseNode> specific(root);
		shared_ptr<Node> base = specific;
		return any(base);
	}

	any visitArithmeticExpr(KodaParser::ArithmeticExprContext* ctx) override {
		shared_ptr<Node> lhs = fetch(ctx->lhs);
		shared_ptr<Node> rhs = fetch(ctx->rhs);
		BinArithOperator op = string_to_binary_arith_operator[ctx->op->getText()];
		return wrap<BinArithNode>(lhs, rhs, op);
	}

	any visitParenthesesAtom(KodaParser::ParenthesesAtomContext* ctx) override { return visit(ctx->expression()); }

	any visitFuncCallAtom(KodaParser::FuncCallAtomContext* ctx) override {
		string name = ctx->ID()->getText();
		vector<shared_ptr<Node>> params;
		for (auto p : ctx->expression()) {
			params.push_back(fetch(p));
		}
		return wrap<FuncCallNode>(name, params);
	}

	any visitStringAtom(KodaParser::StringAtomContext* ctx) override {
		string value = ctx->STRING()->getText();
		// remove quotes
		value = value.substr(1, value.size() - 2);
		return wrap<StringNode>(value);
	}

	any visitBooleanAtom(KodaParser::BooleanAtomContext* ctx) override {
		bool value = ctx->BOOLEAN()->getText() == "true";
		return wrap<BooleanNode>(value);
	}

	any visitNumberAtom(KodaParser::NumberAtomContext* ctx) override {
		auto text = ctx->NUMBER()->getText();
		if (text.find('.') != string::npos) {
			// float
			double value = std::stod(ctx->NUMBER()->getText());
			return wrap<FloatNode>(std::move(value));
		} else {
			// int64
			int64_t value = std::stoll(ctx->NUMBER()->getText());
			return wrap<Int64Node>(std::move(value));
		}
	}

	any visitVoidAtom(KodaParser::VoidAtomContext* ctx) override { return wrap<VoidNode>(); }

	any visitIDAtom(KodaParser::IDAtomContext* ctx) override {
		string name = ctx->ID()->getText();
		return wrap<IDNode>(name);
	}

	any visitVarDef(KodaParser::VarDefContext* ctx) override {
		TypeKind type = string_to_type_kind[ctx->type->getText()];
		string name = ctx->name->getText();

		return wrap<VarDefNode>(type, name);
	}

	any visitVarAssign(KodaParser::VarAssignContext* ctx) override {
		string name = ctx->name->getText();
		shared_ptr<Node> value = fetch(ctx->expression());

		return wrap<VarAssignNode>(name, value);
	}

	any visitVarDefAndAssign(KodaParser::VarDefAndAssignContext* ctx) override {
		TypeKind type = string_to_type_kind[ctx->type->getText()];
		string name = ctx->name->getText();
		shared_ptr<Node> value = fetch(ctx->expression());

		return wrap<VarDefAndAssignNode>(type, name, value);
	}

	any visitFuncDef(KodaParser::FuncDefContext* ctx) override {
		string func_name = ctx->name->getText();
		vector<pair<TypeKind, string>> params;

		antlr4::tree::TerminalNode* last = nullptr;
		for (int i=1; i < ctx->ID().size(); i++) {
			auto id = ctx->ID(i);
			if (last == nullptr) {
				last = id;
			} else {
				std::cout << "type is " << last->getText() << ", name is " << id->getText() << "\n";
				params.push_back(pair(string_to_type_kind[last->getText()], id->getText()));
				last = nullptr;
			}
		}
		
		for (auto p : params) {
			std::cout << "Param: " << p.second << " of type " << type_kind_to_string[p.first] << "\n";
		}


		TypeKind return_type = string_to_type_kind[ctx->ret_type->getText()];
		shared_ptr<BodyNode> body = dynamic_pointer_cast<BodyNode>(fetch(ctx->block()));

		return wrap<FuncDefNode>(func_name, params, return_type, body);
	}
};