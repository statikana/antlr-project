#include "antlr4-runtime.h"

#include "gen_cpp/KodaBaseVisitor.h"
#include "gen_cpp/KodaLexer.h"
#include "gen_cpp/KodaParser.h"

#include "src/scope.cpp"
#include "src/typing/atom.h"

#include <iostream>

using namespace antlr4;

using std::any;
using std::any_cast;
using std::get;
using std::shared_ptr;
using std::make_shared;
using std::move;
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

	any visitAssignment(KodaParser::AssignmentContext* ctx) override {
		std::cout << "assign" << std::endl;
		string id  = ctx->ID()->getText();
		Atom value = unwrap(visit(ctx->expression()));

		scope.define(id, value);
		return nullptr;
	}

	any visitFunctionDefinition(KodaParser::FunctionDefinitionContext* ctx) override {
		string func_name = ctx->func_name->getText();
		auto params = ctx->params->children;

		vector<TypeKind> param_ts;
		vector<string> param_names;

		Node* last = nullptr;
		for (Node* param : params) {
			string param_txt = param->getText();
			if (param_txt == "," | param_txt == "(" | param_txt == ")") { continue; }
			if (last == nullptr) { last = param; continue; }

			TypeKind param_t = string_to_type_kind[last->getText()];
			string param_name = param->getText();

			param_ts.push_back(param_t);
			param_names.push_back(param_name);
		}	
		
		TypeKind return_t = string_to_type_kind[ctx->ret_t->getText()];
		const Func& func = Func(param_ts, param_names, return_t);
		const Atom& atom = make_shared<Func>(move(func));
		scope.define(func_name, atom);
		scope.print();
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
		auto* args = ctx->args;

		// TODO: arg unwrapping
		// TODO: typing
		return nullptr;
	}
};

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		std::cerr << "Error: No input file provided." << std::endl;
		return 1;
	}

	std::ifstream input_file(argv[1]);

	if (!input_file) {
		std::cerr << "Error: could not open file: " << argv[1] << std::endl;
		return 1;
	}
	ANTLRInputStream input(input_file);
	KodaLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	KodaParser parser(&tokens);
	KodaParser::ProgramContext* tree = parser.program();

	auto visitor = MainKodaVisitor();

	visitor.visit(tree);

	std::cout << "Parse complete." << std::endl;
	return 0;
};