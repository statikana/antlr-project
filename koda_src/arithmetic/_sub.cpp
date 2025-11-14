#pragma once
#include "../koda_values.cpp"
#include <utility>


struct SubVisitor {
    public:
        const Instance& rhs;
        SubVisitor(const Instance& other) : rhs(other) {}

        Instance operator()(const std::monostate lhs) {
            throw "unexpected operation";
        }

        Instance operator()(const bool lhs) {
            if (std::holds_alternative<bool>(rhs.data)) {
                return Instance::new_int(lhs - static_cast<int>(std::get<bool>(rhs.data)));
            } else if (std::holds_alternative<int>(rhs.data)) {
                return Instance::new_int(lhs - std::get<int>(rhs.data));
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

        Instance operator()(const ArrayDataPtr lhs) {
            ArrayData new_arr;
            new_arr.resize(lhs->size());

            for (int i=0; i<new_arr.size(); i++) {
                Instance result = std::visit(*this, lhs->at(i)->data);
                new_arr[i] = std::make_shared<Instance>(std::move(result));
            }
            return Instance::new_array(new_arr);
        }
    
        Instance operator()(const StructDataPtr lhs) {
            throw "unexpected operation";
        }
};
