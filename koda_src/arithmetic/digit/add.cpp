#pragma once
#include "../../koda_values.cpp"
#include <utility>


struct AddVisitor {
    public:
        const Instance& rhs;
        AddVisitor(const Instance& other) : rhs(other) {}

        Instance operator()(const std::monostate lhs) {
            throw "unexpected operation";
        }

        Instance operator()(const BoolT lhs) {
            if (holds<BoolT>(rhs.data)) {
                return Instance::new_int(lhs + static_cast<IntT>(get<BoolT>(rhs.data)));
            } else if (holds<IntT>(rhs.data)) {
                return Instance::new_int(lhs + get<IntT>(rhs.data));
            } else if (holds<FloatT>(rhs.data)) {
                return Instance::new_float(static_cast<FloatT>(lhs) + get<FloatT>(rhs.data));
            } 
            throw "unexpected operation";
        }

        Instance operator()(const IntT lhs) {
            if (holds<std::monostate>(rhs.data)) {
                throw "unexpected operation";
            } else if (holds<BoolT>(rhs.data)) {
                return Instance::new_int(lhs + static_cast<IntT>(get<BoolT>(rhs.data)));
            } else if (holds<IntT>(rhs.data)) {
                return Instance::new_int(lhs + get<IntT>(rhs.data));
            } else if (holds<FloatT>(rhs.data)) {
                return Instance::new_float(static_cast<FloatT>(lhs) + get<FloatT>(rhs.data));
            }
            throw "unexpected operation";
        }

        Instance operator()(const FloatT lhs) {
            if (holds<std::monostate>(rhs.data)) {
                throw "unexpected operation";
            } else if (holds<BoolT>(rhs.data)) {
                return Instance::new_float(lhs + static_cast<FloatT>(get<BoolT>(rhs.data)));
            } else if (holds<IntT>(rhs.data)) {
                return Instance::new_float(lhs + get<IntT>(rhs.data));
            } else if (holds<FloatT>(rhs.data)) {
                return Instance::new_float(lhs + get<FloatT>(rhs.data));
            } else if (holds<StringT>(rhs.data)) {
                return Instance::new_string(std::to_string(lhs) + get<StringT>(rhs.data));
            }
            throw "unexpected operation";
        }

        Instance operator()(const StringT lhs) {
            // will throw if .to_text for rhs isn't defined for text
            return Instance::new_string(lhs + rhs.to_text());
        }
        
        Instance operator()(const ArrayDataPtr lhs) {
            ArrayData new_arr;
            new_arr.resize(lhs->size());

            for (size_t i=0; i<new_arr.size(); i++) {
                Instance result = std::visit(*this, lhs->at(i)->data);
                new_arr[i] = std::make_shared<Instance>(std::move(result));
            }
            return Instance::new_array(new_arr);
        }
    
        Instance operator()(const StructDataPtr lhs) {
            throw "unexpected operation";
        }
};
