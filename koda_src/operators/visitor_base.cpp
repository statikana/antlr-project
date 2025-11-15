#pragma once
#include "../koda_values.cpp"

std::string MSG_BAD_CALL = "unexpected operation";

class VisitorBase {
	public:
	const Instance& rhs;
	VisitorBase(const Instance& other) : rhs(other) {}

	Instance operator()(const VoidT lhs) { throw MSG_BAD_CALL; };

	Instance operator()(const BoolT lhs) { throw MSG_BAD_CALL; };

	Instance operator()(const IntT lhs) { throw MSG_BAD_CALL; };

	Instance operator()(const FloatT lhs) { throw MSG_BAD_CALL; };

	Instance operator()(const StringT lhs) { throw MSG_BAD_CALL; };

	Instance operator()(const StructData lhs) { throw MSG_BAD_CALL; };

	Instance operator()(const ArrayData lhs) { throw "how did we get here?"; }
};