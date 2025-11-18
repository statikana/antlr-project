#include "antlr4-runtime.h"

#include "gen_cpp/KodaBaseVisitor.h"
#include "gen_cpp/KodaLexer.h"
#include "gen_cpp/KodaParser.h"

#include "src/scope.cpp"
#include "src/typing/atom.h"

#include <algorithm>
#include <cctype>

using namespace antlr4;

using std::any;
using std::any_cast;
using std::get;
using std::shared_ptr;
using std::string;

typedef antlr4::tree::ParseTree Node;

class MainKodaVisitor : public KodaBaseVisitor {
	private:
	Scope scope = Scope();

	template<typename T> any wrap(const T& obj) { return any(Atom(obj)); }

	Atom unwrap(const any& v) { return any_cast<Atom>(v); }

	public:
	//
	// statement rules
	//
	any visitAssignment(KodaParser::AssignmentContext* ctx) override {
		string id  = ctx->ID()->getText();
		Atom value = unwrap(visit(ctx->expression()));

		scope.define(id, value);
		return nullptr;
	}


	any visitFunctionDefinition(KodaParser::FunctionDefinitionContext* ctx) override {
		string func_name							 = ctx->func_name->getText();
		auto* params = ctx->params;

		// TODO: param unwrapping
		// TODO: function definition
	}

	//
	// expression rules
	//
	any visitArithmeticExpr(KodaParser::ArithmeticExprContext* ctx) override {
		Atom lhs = unwrap(visit(ctx->lhs));
		Atom rhs = unwrap(visit(ctx->rhs));

		// TODO: math inference
		return wrap(Int64(42));
	}

	any visitFunctionCall(KodaParser::FunctionCallContext* ctx) override {
		string func_name						  = ctx->func_name->getText();
		auto* args = ctx->args;

		// TODO: arg unwrapping
		// TODO: typing
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