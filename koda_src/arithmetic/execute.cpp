#pragma once
#include "digit/add.cpp"
#include "digit/div.cpp"
#include "digit/fld.cpp"
#include "digit/mod.cpp"
#include "digit/mul.cpp"
#include "digit/sub.cpp"

Instance infer_arithmetic(const Instance &lhs, std::string str_op,
                          const Instance &rhs) {

  // digit
  if (str_op == "+") {
    AddVisitor visitor(rhs);
    return std::visit(visitor, lhs.data);
  } else if (str_op == "-") {
    SubVisitor visitor(rhs);
    return std::visit(visitor, lhs.data);
    ;
  } else if (str_op == "*") {
    MulVisitor visitor(rhs);
    return std::visit(visitor, lhs.data);
    ;
  } else if (str_op == "/") {
    DivVisitor visitor(rhs);
    return std::visit(visitor, lhs.data);
    ;
  } else if (str_op == "%") {
    ModVisitor visitor(rhs);
    return std::visit(visitor, lhs.data);
  } else if (str_op == "\\") {
    FldVisitor visitor(rhs);
    return std::visit(visitor, lhs.data);
  }
  throw "unexpected operation";
}