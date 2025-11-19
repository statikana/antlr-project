#pragma once

#include "antlr4-runtime.h"
#include "../../gen_cpp/KodaLexer.h"
#include "../../gen_cpp/KodaParser.h"

#include "../parsing/visitor.cpp"
#include "./ast_node.h"

#include <iostream>


ASTNode generate_ast(std::ifstream input_file) {
	ANTLRInputStream input(input_file);
	KodaLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	KodaParser parser(&tokens);
	KodaParser::ProgramContext* tree = parser.program();

	auto visitor = MainKodaVisitor();
};