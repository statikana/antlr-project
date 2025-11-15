#include "antlr4-runtime.h"
#include "gen_cpp/KodaBaseVisitor.h"
#include "gen_cpp/KodaLexer.h"
#include "gen_cpp/KodaParser.h"

#include "koda_src/koda_scope.cpp"
#include "koda_src/koda_values.cpp"
#include "koda_src/operators/execute.cpp"
#include <cctype>
#include <algorithm>

#include "Eigen/Dense"

using namespace antlr4;

typedef antlr4::tree::ParseTree Node;
typedef antlr4::tree::TerminalNode Atom; // every terminal node is an atom

class MainKodaVisitor : public KodaBaseVisitor {

	// for now, only use one scope.
	// will create scope manager later?
	Scope scope = Scope();

	public:
	std::any visitAssignment(KodaParser::AssignmentContext* ctx) override {
		std::string id = ctx->ID()->getText();
		std::string operation;
		if (ctx->op == nullptr) {
			operation = "";
		} else {
			operation = ctx->op->getText();
		}

		std::any rhs_any = visit(ctx->expression());
		Instance rhs	 = std::any_cast<Instance>(rhs_any);

		// no operation, just assign
		if (operation.empty()) {
			scope.define(id, rhs);
		} else {
			if (!scope.is_defined(id)) {
				throw "Undefined name " + id;
			}
			Instance new_value = infer_operation(scope[id], operation, rhs);
			scope.define(id, new_value);
		}

		scope.print();
		return nullptr;
	}

	// EXPRESSIONS
	// Expressions and Atoms always return an Instance wrapped in std::any
	std::any visitArithmeticExpr(KodaParser::ArithmeticExprContext* ctx) override {

		Instance lhs	   = std::any_cast<Instance>(visit(ctx->lhs));
		Instance rhs	   = std::any_cast<Instance>(visit(ctx->rhs));
		std::string op_str = ctx->op->getText();

		return infer_operation(lhs, op_str, rhs);
	}

	std::any visitBitwiseExpr(KodaParser::BitwiseExprContext* ctx) override {
		Instance lhs	   = std::any_cast<Instance>(visit(ctx->lhs));
		Instance rhs	   = std::any_cast<Instance>(visit(ctx->rhs));
		std::string op_str = ctx->op->getText();

		return infer_operation(lhs, op_str, rhs);
	}

	std::any visitComparisonExpr(KodaParser::ComparisonExprContext* ctx) override {
		Instance lhs	   = std::any_cast<Instance>(visit(ctx->lhs));
		Instance rhs	   = std::any_cast<Instance>(visit(ctx->rhs));
		std::string op_str = ctx->op->getText();

		return infer_operation(lhs, op_str, rhs);
	}

	std::any visitSingletonOperatorExpr(KodaParser::SingletonOperatorExprContext* ctx) override {
		Instance lhs = std::any_cast<Instance>(visit(ctx->expression()));
		Instance rhs = Instance::new_void();
		std::string op_str = ctx->op->getText();

		return infer_operation(lhs, op_str, rhs);
	}

	std::any visitAtomExpr(KodaParser::AtomExprContext* ctx) override { return visit(ctx->atom()); }

	// ATOMS
	// Atoms always directly return an Instance wrapped in std::any

	std::any visitParenthesesAtom(KodaParser::ParenthesesAtomContext* ctx) override { return visit(ctx->expression()); }

	std::any visitBooleanAtom(KodaParser::BooleanAtomContext* ctx) override {
		std::string text = ctx->getText();
		return Instance::new_bool(text == "true");
	}

	std::any visitNumberAtom(KodaParser::NumberAtomContext* ctx) override {
		std::string text = ctx->NUMBER()->getText();
		if (text.find(".") == std::string::npos) {
			return std::any(Instance::new_int(stoi(text)));
		} else {
			return std::any(Instance::new_float(stold(text)));
		}
	}

	std::any visitHexAtom(KodaParser::HexAtomContext* ctx) override {
		std::string hex = ctx->HEX()->toString();
		hex.erase(0, 2);	// 0x
		return std::any(Instance::new_int(stoi(hex, nullptr, 16)));
	}

	std::any visitBinaryAtom(KodaParser::BinaryAtomContext* ctx) override {
		std::string bin = ctx->BIN()->toString();
		bin.erase(0, 2);	// 0b
		return std::any(Instance::new_int(stoi(bin, nullptr, 2)));
	}

	std::any visitIDAtom(KodaParser::IDAtomContext* ctx) override {
		std::string id = ctx->ID()->getText();

		if (!scope.is_defined(id)) {
			std::cerr << "ID '" << id << "' not defined." << std::endl;
			throw;
		}
		return std::any(scope[id]);
	}

	std::any visitStringAtom(KodaParser::StringAtomContext* ctx) override {
		std::string text = ctx->STRING()->getText();
		text.erase(0, 1);
		text.erase(text.size() - 1);
		return std::any(Instance::new_string(text));
	}

	std::any visitArrayAtom(KodaParser::ArrayAtomContext* ctx) override {

		// includes token nodes for ','
		Node* internals = ctx->array_inside();

		std::vector<Node*> value_nodes;

		std::copy_if(
			internals->children.begin(), internals->children.end(), std::back_inserter(value_nodes), [](Node* v) {
				return v->getTreeType() == tree::ParseTreeType::RULE;
			});

		// evaulated: holds the visited evaluations of expressions in value_nodes
		// should be same size
		ArrayData evaluated;
		evaluated.reserve(value_nodes.size());

		// evaluated.size() is still 0 here, but there is memory for it to expand
		// into up to value_nodes.size()
		for (int i = 0; i < value_nodes.size(); i++) {
			Instance result						   = std::any_cast<Instance>(visit(value_nodes[i]));
			std::shared_ptr<Instance> instance_ptr = std::make_shared<Instance>(std::move(result));
			evaluated.push_back(instance_ptr);
		}

		return Instance::new_array(evaluated);
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

	MainKodaVisitor visitor = MainKodaVisitor();

	visitor.visit(tree);

	std::cout << "Parse complete." << std::endl;
	return 0;
};