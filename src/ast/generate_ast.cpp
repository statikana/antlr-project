#pragma once

#include "../../gen_cpp/KodaLexer.h"
#include "../../gen_cpp/KodaParser.h"
#include "antlr4-runtime.h"

#include "./ast_node.h"
#include "./ast_visitor.cpp"

#include <iostream>

using namespace antlr4;

shared_ptr<BodyNode> generate_ast(std::ifstream& input_file) {
	ANTLRInputStream input(input_file);
	KodaLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	KodaParser parser(&tokens);
	KodaParser::ProgramContext* tree = parser.program();

	auto visitor = AstVisitor();

	auto program = any_cast<shared_ptr<Node>>(visitor.visit(tree));
	auto body = dynamic_pointer_cast<BodyNode>(program);
	return body;
};