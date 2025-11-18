#pragma once
#include <stdexcept>
#include <string>

// Exception used for unimplemented operations
class NotImplementedT : public std::runtime_error {
	std::string what_message = "Operation not implemented";

	public:
	NotImplementedT() : std::runtime_error(what_message) {}

	const char* what() const noexcept override { return what_message.c_str(); }
};

inline NotImplementedT not_implemented = NotImplementedT();