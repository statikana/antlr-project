#pragma once
#include "../koda_values.cpp"
#include <utility>

struct AddVisitor {
    public:
        const Instance& rhs;
        AddVisitor(const Instance& other) : rhs(other) {}

        Instance operator()(const std::monostate lhs) {
            throw "unexpected operation";
        }

        Instance operator()(const bool lhs) {
            if (std::holds_alternative<bool>(rhs.data)) {
                return Instance::new_int(lhs + static_cast<int>(std::get<bool>(rhs.data)));
            } else if (std::holds_alternative<int>(rhs.data)) {
                return Instance::new_int(lhs + std::get<int>(rhs.data));
            } else if (std::holds_alternative<long double>(rhs.data)) {
                return Instance::new_float(static_cast<long double>(lhs) + std::get<long double>(rhs.data));
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
            // will throw if .to_text for rhs isn't defined for text
            return Instance::new_string(lhs + rhs.to_text());
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
