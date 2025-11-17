#include "antlr4-runtime.h"
#include "gen_cpp/KodaBaseVisitor.h"
#include "gen_cpp/KodaLexer.h"
#include "gen_cpp/KodaParser.h"

#include "src/scope.cpp"
#include "src/types/object.h"
#include "src/types/std/bool.h"
#include "src/types/std/int64.h"
#include "src/types/std/void.h"
#include "src/util/operate.cpp"

#include <algorithm>
#include <cctype>

using namespace antlr4;

typedef antlr4::tree::ParseTree Node;
typedef antlr4::tree::TerminalNode Atom; // every terminal node is an atom

class MainKodaVisitor : public KodaBaseVisitor {

	// for now, only use global variable.
	// will create scope manager later?
	Scope scope = Scope();

	public:
	// ID operator ASSIGNMENT expression
	// e.g. x += 5
	std::any visitAssignment(KodaParser::AssignmentContext* ctx) override {
		std::string id = ctx->ID()->getText();
		std::string operation;
		if (ctx->op == nullptr) {
			operation = "";
		} else {
			operation = ctx->op->getText();
		}

		std::any rhs_any = visit(ctx->expression());
		Object rhs		 = std::any_cast<Object>(rhs_any);

		// no operation, just assign
		if (operation.empty()) {
			scope.define(id, rhs);
		} else {
			if (!scope.is_defined(id)) {
				throw "Undefined name " + id;
			}
			Object new_value = operate(scope[id], operation, rhs);
			scope.define(id, new_value);
		}

		scope.print();
		return nullptr;
	}

	//
	// EXPRESSIONS
	//
	// Expressions and Atoms always return an Object wrapped in std::any

	// lhs op rhs
	// e.g. 5 + 3
	std::any visitArithmeticExpr(KodaParser::ArithmeticExprContext* ctx) override {

		Object lhs		   = std::any_cast<Object>(visit(ctx->lhs));
		Object rhs		   = std::any_cast<Object>(visit(ctx->rhs));
		std::string op_str = ctx->op->getText();

		return std::any(operate(lhs, op_str, rhs));
	}

	// lhs bw_operator rhs
	// e.g. 5 & 3
	std::any visitBitwiseExpr(KodaParser::BitwiseExprContext* ctx) override {
		Object lhs		   = std::any_cast<Object>(visit(ctx->lhs));
		Object rhs		   = std::any_cast<Object>(visit(ctx->rhs));
		std::string op_str = ctx->op->getText();

		return std::any(operate(lhs, op_str, rhs));
	}

	// lhs comparer rhs
	// e.g. 5 > 3
	std::any visitComparisonExpr(KodaParser::ComparisonExprContext* ctx) override {
		Object lhs		   = std::any_cast<Object>(visit(ctx->lhs));
		Object rhs		   = std::any_cast<Object>(visit(ctx->rhs));
		std::string op_str = ctx->op->getText();

		return std::any(operate(lhs, op_str, rhs));
	}

	// singleton_operator expression
	// e.g. ~true
	std::any visitSingletonOperatorExpr(KodaParser::SingletonOperatorExprContext* ctx) override {
		Object lhs		   = std::any_cast<Object>(visit(ctx->expression()));
		Void rhs		   = VOID;
		std::string op_str = ctx->op->getText();

		// this code errors because operate expects two Objects,
		// and rhs is Void.
		return std::any(operate(lhs, op_str, rhs));
	}

	std::any visitAtomExpr(KodaParser::AtomExprContext* ctx) override { return visit(ctx->atom()); }

	//
	// ATOMS
	//

	// ( atom )
	std::any visitParenthesesAtom(KodaParser::ParenthesesAtomContext* ctx) override { return visit(ctx->expression()); }

	// boolean
	// e.g. true
	std::any visitBooleanAtom(KodaParser::BooleanAtomContext* ctx) override {
		std::string text = ctx->getText();
		return std::any(Bool(text == "true"));
	}

	// number
	// e.g. 123, 3.14, .1
	std::any visitNumberAtom(KodaParser::NumberAtomContext* ctx) override {
		std::string text = ctx->NUMBER()->getText();
		if (text.find(".") == std::string::npos) {
			return std::any(Int64(stoi(text)));
		} else {
			// return std::any(Float(stold(text)));
			throw "Float type not implemented yet.";
		}
	}

	// hex number
	// e.g. 0x1A3F
	std::any visitHexAtom(KodaParser::HexAtomContext* ctx) override {
		std::string hex = ctx->HEX()->toString();
		hex.erase(0, 2); // 0x
		return std::any(Int64(stoi(hex, nullptr, 16)));
	}

	// binary number
	// e.g. 0b101101
	std::any visitBinaryAtom(KodaParser::BinaryAtomContext* ctx) override {
		std::string bin = ctx->BIN()->toString();
		bin.erase(0, 2); // 0b
		return std::any(Int64(stoi(bin, nullptr, 2)));
	}

	// identifier
	// e.g. foo, bar
	// (caught last in ATOM parsing rules, other keywords take priotity. i.e. ID won't catch "true" or "123")
	std::any visitIDAtom(KodaParser::IDAtomContext* ctx) override {
		std::string id = ctx->ID()->getText();

		if (!scope.is_defined(id)) {
			std::cerr << "ID '" << id << "' not defined." << std::endl;
			throw;
		}
		return std::any(scope[id]);
	}

	// string
	// e.g. "hello world"
	std::any visitStringAtom(KodaParser::StringAtomContext* ctx) override {
		std::string text = ctx->STRING()->getText();
		text.erase(0, 1);
		text.erase(text.size() - 1);
		throw "String type not implemented yet.";
	}

	// array
	// e.g. [1, 2, "hello", true]
	// TODO: enforce monotype arrays
	std::any visitArrayAtom(KodaParser::ArrayAtomContext* ctx) override { throw "Array type not implemented yet."; }
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

	MainKodaVisitor visitor = MainKodaVisitor();

	visitor.visit(tree);

	std::cout << "Parse complete." << std::endl;
	return 0;
};