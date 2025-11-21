// executes an AST tree
#pragma once

#include "../typing/scope.cpp"

#include "../ast/ast_node.h"

#include "./op/add.cpp"
#include "./op/div.cpp"
#include "./op/mul.cpp"
#include "./op/sub.cpp"

#include <map>
#include <memory>

using std::dynamic_pointer_cast;
using std::make_shared;
using std::map;
using std::shared_ptr;


AtomV exec(const shared_ptr<Node>& node, Scope& scope) {
	std::cout << "Executing node...\n";
	std::cout << node->get_text(0) << std::endl;
	scope.print();
	std::cout << std::endl << std::endl;
	// until a return syntax is defined, the last line is also what gets returned.
	if (auto body = dynamic_pointer_cast<BodyNode>(node)) {
		if (scope.get_parent() == nullptr) {
			AtomV prev;
			for (auto n: body->getNodes()) { prev = exec(n, scope); }
			return prev;
		} else {
			AtomV prev;
			Scope* child = scope.enter();
			for (auto n: body->getNodes()) { prev = exec(n, *child); }
			delete child;
			return prev;
		}
	
	} else if (auto id_node = 		dynamic_pointer_cast<IDNode>(node)) {
		return scope.get(id_node->get_name());
	} else if (auto var_def = 		dynamic_pointer_cast<VarDefNode>(node)) {
		if (scope.is_defined(var_def->get_name())) {
			throw std::runtime_error("Variable " + var_def->get_name() + " is already defined.");
		}
		TypeKind reported_type = var_def->get_type();
		AtomV value;
		if (reported_type == VOID_TYPE) {
			value = make_shared<Void>();
		} else {
			// default initialize
			switch (reported_type) {
			case INT64_TYPE:
				value = make_shared<Int64>(0);
				break;
			case FLOAT_TYPE:
				value = make_shared<Float>(0.0);
				break;
			case STRING_TYPE:
				value = make_shared<String>("");
				break;
			case BOOL_TYPE:
				value = make_shared<Bool>(false);
				break;
			default:
				throw std::runtime_error("Cannot default initialize type: " + type_kind_to_string[reported_type]);
			}
		}
		scope.define(var_def->get_name(), value);
		return value;
	
	} else if (auto func_def = 		dynamic_pointer_cast<FuncDefNode>(node)) {
		string name = func_def->get_name();
		if (scope.is_defined(name)) {
			throw std::runtime_error(name + " is already defined");
		}
		scope.define_func(name, pair(make_shared<Scope>(&scope), func_def));
		return make_shared<Void>();
	
	} else if (auto var_assign = 	dynamic_pointer_cast<VarAssignNode>(node)) {
		AtomV value = exec(var_assign->get_value(), scope);
		if (!scope.is_defined(var_assign->get_name())) {
			throw std::runtime_error("Variable " + var_assign->get_name() + " is not defined.");
		}
		if (!type_subtype(AtomVAccess(scope.get(var_assign->get_name())).get_type(), AtomVAccess(value).get_type())) {
			throw std::runtime_error(
				"Cannot assign value of type " + type_kind_to_string[AtomVAccess(value).get_type()] + " to variable " +
				var_assign->get_name() + " of type " +
				type_kind_to_string[AtomVAccess(scope.get(var_assign->get_name())).get_type()] + ".");
		}
		scope.define(var_assign->get_name(), value);
		return value;
	} else if (auto var_def_assign =dynamic_pointer_cast<VarDefAndAssignNode>(node)) {
		TypeKind reported_type = var_def_assign->get_type();
		AtomV value;
		if (scope.is_defined(var_def_assign->get_name())) {
			throw std::runtime_error("Variable " + var_def_assign->get_name() + " is already defined.");
		}
		if (reported_type == VOID_TYPE) {
			value = make_shared<Void>();
		} else if (reported_type == AUTO_TYPE) {
			value = exec(var_def_assign->get_value(), scope);
		} else {
			value = exec(var_def_assign->get_value(), scope);
			auto actual_type = AtomVAccess(value).get_type();
			if (!type_subtype(reported_type, actual_type)) {
				std::cerr << "Actual type cannot fit in reported";
				throw std::runtime_error("Actual type cannot fit in reported type.");
			}
		}

		scope.define(var_def_assign->get_name(), value);
		return value;

	} else if (auto func_call = dynamic_pointer_cast<FuncCallNode>(node)) {
		string func_name = func_call->get_name();
		if (!scope.is_func_defined(func_name)) {
			throw std::runtime_error("Function ")
		}
	}
	else if (auto bin_arith = 	dynamic_pointer_cast<BinArithNode>(node)) {
		AtomV lhs = exec(bin_arith->getLHS(), scope);
		AtomV rhs = exec(bin_arith->getRHS(), scope);
		switch (bin_arith->getOp()) {
		case ADD:
			return add(lhs, rhs);
		case SUB:
			return sub(lhs, rhs);
		case MUL:
			return mul(lhs, rhs);
		case DIV:
			return div(lhs, rhs);
		default:
			throw std::runtime_error("Unknown binary arithmetic operator");
		}
	} else if (auto boolean_node = 	dynamic_pointer_cast<BooleanNode>(node)) {
		return make_shared<Bool>(std::move(boolean_node->get_value()));
	} else if (auto int64_node = 	dynamic_pointer_cast<Int64Node>(node)) {
		return make_shared<Int64>(std::move(int64_node->get_value()));
	} else if (auto float_node = 	dynamic_pointer_cast<FloatNode>(node)) {
		return make_shared<Float>(std::move(float_node->get_value()));
	} else if (auto void_node = 	dynamic_pointer_cast<VoidNode>(node)) {
		return make_shared<Void>();
	} else if (auto string_node = 	dynamic_pointer_cast<StringNode>(node)) {
		return make_shared<String>(string_node->get_value());
	} else if (auto if_else_node = 	dynamic_pointer_cast<IfElseNode>(node)) {
		AtomV cond = exec(if_else_node->getCondition(), scope);
		if (AtomVAccess(cond).get_truth()) {
			std::cout << "Condition: " << AtomVAccess(cond).get_text() << " is true, executing if body.\n";
			return exec(if_else_node->getIfBody(), scope);
		} else {
			std::cout << "Condition: " << AtomVAccess(cond).get_text() << " is false, executing else body.\n";
			return exec(if_else_node->getElseBody(), scope);
		}
	} else {
		throw "Unknown node type.";
	}
}


struct FuncBody {
	BodyNode* body;
	Scope* scope;
};
