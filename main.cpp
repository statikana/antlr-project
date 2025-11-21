#include <iostream>

#include "src/ast/generate_ast.cpp"
#include "src/exec/exec.cpp"

#include <memory>

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

	auto program = generate_ast(input_file);
	std::cout << "... begin generated AST..." << std::endl;
	std::cout << program->get_text() << std::endl;
	std::cout << "... end generated AST..." << std::endl;

	Scope root = Scope();
	auto r = exec(program, root);
	std::cout << AtomVAccess(r).get_text() << std::endl;
	root.print();
	return 0;
};