#include <iostream>
#include "antlr4-runtime.h"
#include "gen_cpp/KodaLexer.h"
#include "gen_cpp/KodaParser.h"

#include "src/ast/generate_ast.cpp"

using namespace antlr4;


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

    generate_ast(input_file);

	std::cout << "Parse complete." << std::endl;
	return 0;
};