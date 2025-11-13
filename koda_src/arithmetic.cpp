#pragma once
#include "koda_values.cpp"
#include <variant>
#include <map>

// defined basic arithmetic for the root builtin types:
//      void, bool, int, float, string

struct AddVisitor {
    public:
        const Instance& rhs;
        AddVisitor(const Instance& other) : rhs(other) {}

        Instance operator()(const std::monostate lhs) {
            throw "unexpected operation";
        }

        Instance operator()(const bool lhs) {
            if (std::holds_alternative<std::monostate>(rhs.data)) {
                throw "unexpected operation";
            } else if (
                std::holds_alternative<bool>(rhs.data) |
                std::holds_alternative<int>(rhs.data)
            ) { // cast bool to int
                return Instance::new_int(static_cast<int>(lhs) + std::get<int>(rhs.data));
            } else if (std::holds_alternative<long double>(rhs.data)) {
                return Instance::new_float(static_cast<long double>(lhs) + std::get<long double>(rhs.data));
            } else if (std::holds_alternative<std::string>(rhs.data)) {
                return Instance::new_string(std::to_string(lhs) + std::get<std::string>(rhs.data));
            }
            throw "unexpected operation";
        }

        Instance operator()(const int lhs) {
            if (std::holds_alternative<std::monostate>(rhs.data)) {
                throw "unexpected operation";
            } else if (std::holds_alternative<bool>(rhs.data)) {
                return Instance::new_int(lhs + static_cast<int>(std::get<bool>(rhs.data)));
            } else if (std::holds_alternative<int>(rhs.data)) {
                return Instance::new_int(lhs + std::get<int>(rhs.data));
            } else if (std::holds_alternative<long double>(rhs.data)) {
                return Instance::new_float(static_cast<long double>(lhs) + std::get<long double>(rhs.data));
            } else if (std::holds_alternative<std::string>(rhs.data)) {
                return Instance::new_string(std::to_string(lhs) + std::get<std::string>(rhs.data));
            }
            throw "unexpected operation";
        }

        Instance operator()(const long double lhs) {
            if (std::holds_alternative<std::monostate>(rhs.data)) {
                throw "unexpected operation";
            } else if (std::holds_alternative<bool>(rhs.data)) {
                return Instance::new_float(lhs + static_cast<long double>(std::get<bool>(rhs.data)));
            } else if (std::holds_alternative<int>(rhs.data)) {
                return Instance::new_float(lhs + std::get<int>(rhs.data));
            } else if (std::holds_alternative<long double>(rhs.data)) {
                return Instance::new_float(lhs + std::get<long double>(rhs.data));
            } else if (std::holds_alternative<std::string>(rhs.data)) {
                return Instance::new_string(std::to_string(lhs) + std::get<std::string>(rhs.data));
            }
            throw "unexpected operation";
        }

        Instance operator()(const std::string lhs) {
            // will throw if .to_text for rhs isn't defined
            return Instance::new_string(lhs + rhs.to_text());
        }
};

struct SubVisitor {
    public:
        const Instance& rhs;
        SubVisitor(const Instance& other) : rhs(other) {}

        Instance operator()(const std::monostate lhs) {
            throw "unexpected operation";
        }

        Instance operator()(const bool lhs) {
            if (std::holds_alternative<bool>(rhs.data)) {
                return Instance::new_int(static_cast<int>(lhs) - static_cast<int>(std::get<bool>(rhs.data)));
            } else if (std::holds_alternative<int>(rhs.data)) {
                return Instance::new_int(static_cast<int>(lhs) - std::get<int>(rhs.data));
            } else if (std::holds_alternative<long double>(rhs.data)) {
                return Instance::new_float(static_cast<long double>(lhs) - std::get<long double>(rhs.data));
            }
            throw "unexpected operation";
        }

        Instance operator()(const int lhs) {
            if (std::holds_alternative<bool>(rhs.data)) {
                return Instance::new_int(lhs - static_cast<int>(std::get<bool>(rhs.data)));
            } else if (std::holds_alternative<int>(rhs.data)) {
                return Instance::new_int(lhs - std::get<int>(rhs.data));
            } else if (std::holds_alternative<long double>(rhs.data)) {
                return Instance::new_float(static_cast<long double>(lhs) - std::get<long double>(rhs.data));
            }
            throw "unexpected operation";
        }

        Instance operator()(const long double lhs) {
            if (std::holds_alternative<bool>(rhs.data)) {
                return Instance::new_float(lhs - static_cast<long double>(std::get<bool>(rhs.data)));
            } else if (std::holds_alternative<int>(rhs.data)) {
                return Instance::new_float(lhs - std::get<int>(rhs.data));
            } else if (std::holds_alternative<long double>(rhs.data)) {
                return Instance::new_float(lhs - std::get<long double>(rhs.data));
            }
            throw "unexpected operation";
        } 

        Instance operator()(const std::string lhs) {
            if (!std::holds_alternative<std::string>(rhs.data)) {
                throw "unexpected operation";
            }

            // string truncation: "foobar" - "bar"  = "foo"
            // for clarity, only defined for str - str (unlike string concatenation, where "foo" + 5 = "foo5")
            std::string rhs_str = std::get<std::string>(rhs.data);

            auto start = lhs.find(rhs_str);
            if (start == std::string::npos) {
                return Instance::new_string(lhs);
            } else {
                return Instance::new_string(lhs.substr(0, start));
            }
        }
};

using Visitor = std::variant<
    AddVisitor,
    SubVisitor
>;

Instance infer_arithmetic(const Instance& lhs, std::string str_op, const Instance& rhs) {
    if (str_op == "+") {
        AddVisitor visitor(rhs);
        return std::visit(visitor, lhs.data);
    } else if (str_op == "-") {
        SubVisitor visitor(rhs);
        return std::visit(visitor, lhs.data);;
    }
    throw "unexpected operation";
}