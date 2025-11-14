#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>
#include <memory>

#include "antlr4-runtime.h"
#include "gen_cpp/mainLexer.h"
#include "gen_cpp/mainParser.h"
#include "gen_cpp/mainBaseVisitor.h"

#include "koda_src/koda_values.cpp"
#include "koda_src/koda_scope.cpp"
#include "koda_src/arithmetic/execute.cpp"

using namespace antlr4;

typedef antlr4::tree::ParseTree Node;
typedef antlr4::tree::TerminalNode Atom; // every terminal node is an atom

class KodaVisitor : public mainBaseVisitor {
    
    // for now, only use one scope.
    // will create scope manager later?
    Scope scope = Scope();

    public:
        std::any visitAssignment(mainParser::AssignmentContext* ctx) override {
            std::string id = ctx->ID()->getText();
            std::string operation;
            if (ctx->op == nullptr) {
                operation = "";
            } else {
                operation = ctx->op->getText();
            }

            std::any rhs_any = visit(ctx->expression());
            Instance rhs = std::any_cast<Instance>(rhs_any);
            

            // no operation, just assign
            if (operation.empty()) {
                scope.define(id, rhs);
            } else {
                if (!scope.is_defined(id)) {
                    throw "Undefined name " + id;
                }
                Instance new_value = infer_arithmetic(scope[id], operation, rhs);
                scope.define(id, new_value);
            }

            scope.print();
            return nullptr;
        }
        

        // EXPRESSIONS
        // Expressions and Atoms always return an Instance wrapped in std::any
        std::any visitArithmeticExpr(mainParser::ArithmeticExprContext* ctx) override {

            Instance lhs = std::any_cast<Instance>(visit(ctx->lhs));
            Instance rhs = std::any_cast<Instance>(visit(ctx->rhs));

            std::string op_str= ctx->op->getText();


            return infer_arithmetic(lhs, op_str, rhs);
        }

        std::any visitBitwiseExpr(mainParser::BitwiseExprContext* ctx) override {
            Instance lhs = std::any_cast<Instance>(visit(ctx->lhs));
            Instance rhs = std::any_cast<Instance>(visit(ctx->rhs));
            std::string op_str = ctx->op->getText();

            throw "not implemented";
        }

        std::any visitComparisonExpr(mainParser::ComparisonExprContext* ctx) override {
            Instance lhs = std::any_cast<Instance>(visit(ctx->lhs));
            Instance rhs = std::any_cast<Instance>(visit(ctx->rhs));
            std::string op_str = ctx->op->getText();

            throw "not implemented";
        }

        std::any visitAtomExpr(mainParser::AtomExprContext* ctx) override {
            return visit(ctx->atom());
        }
        
        // ATOMS
        // Atoms always directly return an Instance wrapped in std::any

        std::any visitBooleanAtom(mainParser::BooleanAtomContext* ctx) override {
            std::string text = ctx->getText();
            if (text.compare("true") == 0) {
                return Instance::new_bool(true);
            }
            return Instance::new_bool(false);
        }
        std::any visitNumberAtom(mainParser::NumberAtomContext* ctx) override {
            std::string text = ctx->NUMBER()->getText();
            if (text.find(".") == std::string::npos) {
                return std::any(Instance::new_int(stoi(text)));
            } else {
                return std::any(Instance::new_float(stold(text)));
            }
        }

        std::any visitStringAtom(mainParser::StringAtomContext* ctx) override {
            std::string text = ctx->STRING()->getText();
            text.erase(0, 1);
            text.erase(text.size() - 1);
            return std::any(Instance::new_string(text));
        }

        std::any visitIDAtom(mainParser::IDAtomContext* ctx)override {
            std::string id = ctx->ID()->getText();

            if (!scope.is_defined(id)) {
                std::cerr << "ID '" << id << "' not defined." << std::endl;
                throw;
            }
            return std::any(scope[id]);
        }

        std::any visitParenthesesAtom(mainParser::ParenthesesAtomContext* ctx) override {
            return visit(ctx->expression());
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
    mainLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    mainParser parser(&tokens);
    mainParser::ProgramContext *tree = parser.program();

    KodaVisitor visitor = KodaVisitor();

    visitor.visit(tree);

    std::cout << "Parse complete." << std::endl;
    return 0;
};