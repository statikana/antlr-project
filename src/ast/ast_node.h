// represents an operation or value
#pragma once
// #include "../typing/atom.h"
#include <vector>
#include <string>
#include <map>
#include <variant>

using std::vector;
using std::string;
using std::variant;


using Node = variant<
    BinaryOpNode*,
    UnaryOpNode*,
    ComparisonNode*,
    WhileNode*,
    IfElseNode*,
    FunctionCallNode*,
    VariableNode*
>;

enum CompOp {
    EQ, LT, LE, GT, GE
};

enum BinaryOp {
    ADD, SUB, MUL, DIV
};

enum UnaryOp {
    NOT
};


struct BinaryOpNode {
    Node lhs;
    Node rhs;
    BinaryOp op;
};

struct UnaryOpNode {
    Node item;
    UnaryOp op;
};

struct ComparisonNode {
    Node lhs;
    Node rhs;
    CompOp op;
};

struct WhileNode {
    ComparisonNode condition;
    Node body;
};

struct IfElseNode {
    ComparisonNode condition;
    Node if_body;
    Node else_body;
};

struct FunctionCallNode {
    string name;
    vector<Node> arguments;
};

struct VariableNode {
    string name;
};

