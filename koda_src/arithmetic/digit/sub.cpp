#pragma once
#include "../../koda_values.cpp"
#include <utility>

struct SubVisitor {
public:
  const Instance &rhs;
  SubVisitor(const Instance &other) : rhs(other) {}

  Instance operator()(const std::monostate lhs) {
    throw "unexpected operation";
  }

  Instance operator()(const BoolT lhs) {
    if (holds<BoolT>(rhs.data)) {
      return Instance::new_int(static_cast<IntT>(lhs) -
                               static_cast<IntT>(get<BoolT>(rhs.data)));
    } else if (holds<IntT>(rhs.data)) {
      return Instance::new_int(lhs - get<IntT>(rhs.data));
    } else if (holds<FloatT>(rhs.data)) {
      return Instance::new_float(static_cast<FloatT>(lhs) -
                                 get<FloatT>(rhs.data));
    }
    throw "unexpected operation";
  }
  Instance operator()(const IntT lhs) {
    if (holds<BoolT>(rhs.data)) {
      return Instance::new_int(lhs - static_cast<IntT>(get<BoolT>(rhs.data)));
    } else if (holds<IntT>(rhs.data)) {
      return Instance::new_int(lhs - get<IntT>(rhs.data));
    } else if (holds<FloatT>(rhs.data)) {
      return Instance::new_float(static_cast<FloatT>(lhs) -
                                 get<FloatT>(rhs.data));
    }
    throw "unexpected operation";
  }

  Instance operator()(const FloatT lhs) {
    if (holds<BoolT>(rhs.data)) {
      return Instance::new_float(lhs -
                                 static_cast<FloatT>(get<BoolT>(rhs.data)));
    } else if (holds<IntT>(rhs.data)) {
      return Instance::new_float(lhs -
                                 static_cast<FloatT>(get<IntT>(rhs.data)));
    } else if (holds<FloatT>(rhs.data)) {
      return Instance::new_float(lhs - get<FloatT>(rhs.data));
    }
    throw "unexpected operation";
  }

  Instance operator()(const StringT lhs) {
    if (!holds<StringT>(rhs.data)) {
      throw "unexpected operation";
    }

    // string truncation: "foobar" - "bar"  = "foo"
    // for clarity, only defined for str - str (unlike string concatenation,
    // where "foo" + 5 = "foo5")
    StringT rhs_str = get<StringT>(rhs.data);

    auto start = lhs.find(rhs_str);
    if (start == StringT::npos) {
      return Instance::new_string(lhs);
    } else {
      return Instance::new_string(lhs.substr(0, start));
    }
  }

  Instance operator()(const ArrayDataPtr lhs) {
    ArrayData new_arr;
    new_arr.resize(lhs->size());

    for (size_t i = 0; i < new_arr.size(); i++) {
      Instance result = std::visit(*this, lhs->at(i)->data);
      new_arr[i] = std::make_shared<Instance>(std::move(result));
    }
    return Instance::new_array(new_arr);
  }

  Instance operator()(const StructDataPtr lhs) { throw "unexpected operation"; }
};
