#pragma once
#include "antlr4-runtime.h"

#include "../../gen_cpp/KodaBaseVisitor.h"
#include "../../gen_cpp/KodaParser.h"

#include "../typing/scope.cpp"
#include "../typing/atom.h"
#include "../typing/subtype.h"
#include "./ast_node.h"

// visits the tree and creates AST nodes representing the syntax tree.

using std::any;

class AstVisitor : public KodaBaseVisitor {
    any visitProgram(KodaParser::ProgramContext* ctx) override {
        
    }
};