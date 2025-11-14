#pragma once
#include "_add.cpp"
#include "_sub.cpp"
#include "_mul.cpp"
#include "_div.cpp"


Instance infer_arithmetic(const Instance& lhs, std::string str_op, const Instance& rhs) {
    if (str_op == "+") {
        AddVisitor visitor(rhs);
        return std::visit(visitor, lhs.data);
    } else if (str_op == "-") {
        SubVisitor visitor(rhs);
        return std::visit(visitor, lhs.data);;
    } else if (str_op == "*") {
        MulVisitor visitor(rhs);
        return std::visit(visitor, lhs.data);;
    } else if (str_op == "/") {
        DivVisitor visitor(rhs);
        return std::visit(visitor, lhs.data);;
    }
    throw "unexpected operation";
}