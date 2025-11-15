#pragma once

#include "arithmetic/arith_add.cpp"
#include "arithmetic/arith_div.cpp"
#include "arithmetic/arith_fld.cpp"
#include "arithmetic/arith_mod.cpp"
#include "arithmetic/arith_mul.cpp"
#include "arithmetic/arith_pow.cpp"
#include "arithmetic/arith_sub.cpp"

#include "bitwise/bw_and.cpp"
#include "bitwise/bw_or.cpp"
#include "bitwise/bw_xor.cpp"
#include "bitwise/bw_lshift.cpp"
#include "bitwise/bw_rshift.cpp"

#include "logical/logic_ge.cpp"
#include "logical/logic_gt.cpp"
#include "logical/logic_le.cpp"
#include "logical/logic_lt.cpp"
#include "logical/logic_eq.cpp"
#include "logical/logic_neq.cpp"

#include "single/bw_not.cpp"
#include "single/logic_not.cpp"

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>


template <typename VisitorT>
Instance lift(VisitorT v, const Instance& lhs) {
	if (lhs.type->get_kind() == ARRAY) {
		std::vector<std::shared_ptr<Instance>> result;

		for (const auto& i : get<ArrayData>(lhs.data)) {
			Instance elem = std::visit(v, i->data);
			result.push_back(std::make_shared<Instance>(std::move(elem)));
		}

		return Instance::new_array(result);
	}
	return std::visit(v, lhs.data);
}

Instance infer_operation(
	const Instance& lhs_c,
	std::string op,
	const Instance& rhs_c
) {
	Instance lhs = lhs_c;
	Instance rhs = rhs_c;

	if (op == "+") {
		return lift(ArithAddVisitor(rhs), lhs);
	} else if (op == "-") {
		return lift(ArithSubVisitor(rhs), lhs);
	} else if (op == "*") {
		return lift(ArithMulVisitor(rhs), lhs);
	} else if (op == "/") {
		return lift(ArithDivVisitor(rhs), lhs);
	} else if (op == "%") {
		return lift(ArithModVisitor(rhs), lhs);
	} else if (op == "**") {
		return lift(ArithPowVisitor(rhs), lhs);
	} else if (op == "&") {
		return lift(BwAndVisitor(rhs), lhs);
	} else if (op == "|") {
		return lift(BwOrVisitor(rhs), lhs);
	} else if (op == "^") {
		return lift(BwXorVisitor(rhs), lhs);
	} else if (op == "<<") {
		return lift(BwLshiftVisitor(rhs), lhs);
	} else if (op == ">>") {
		return lift(BwRshiftVisitor(rhs), lhs);
	} else if (op == ">") {
		return lift(LogicGtVisitor(rhs), lhs);
	} else if (op == "<") {
		return lift(LogicLtVisitor(rhs), lhs);
	} else if (op == ">=") {
		return lift(LogicGeVisitor(rhs), lhs);
	} else if (op == "<=") {
		return lift(LogicLeVisitor(rhs), lhs);
	} else if (op == "==") {
		return lift(LogicEqVisitor(rhs), lhs);
	} else if (op == "!=") {
		return lift(LogicNeqVisitor(rhs), lhs);
	} else if (op == "~") {
		if (rhs.type->get_kind() != VOID) {
			throw "bitwise NOT operator takes no right-hand side operand";
		}
		return lift(BwNotVisitor(rhs), lhs);
	} else if (op == "!") {
		if (rhs.type->get_kind() != VOID) {
			throw "logical NOT operator takes no right-hand side operand";
		}
		return lift(LogicNotVisitor(rhs), lhs);
	} else {
		throw "unknown operation " + op;
	}
}