#pragma once
#include "../../koda_values.cpp"
#include <cmath>
#include <utility>

struct ModVisitor {
public:
  const Instance &rhs;
  ModVisitor(const Instance &other) : rhs(other) {}

  Instance operator()(const std::monostate lhs) {
    throw "unexpected operation";
  }
  Instance operator()(const BoolT lhs) {
    if (holds<BoolT>(rhs.data)) {
      return Instance::new_int(static_cast<IntT>(lhs) %
                               static_cast<IntT>(get<BoolT>(rhs.data)));
    } else if (holds<IntT>(rhs.data)) {
      return Instance::new_int(static_cast<IntT>(lhs) % get<IntT>(rhs.data));
    } else if (holds<FloatT>(rhs.data)) {
      return Instance::new_float(
          std::fmod(static_cast<FloatT>(lhs), get<FloatT>(rhs.data)));
    }
    throw "unexpected operation";
  }
  Instance operator()(const IntT lhs) {
    if (holds<BoolT>(rhs.data)) {
      return Instance::new_int(lhs % static_cast<IntT>(get<BoolT>(rhs.data)));
    } else if (holds<IntT>(rhs.data)) {
      return Instance::new_int(lhs % get<IntT>(rhs.data));
    } else if (holds<FloatT>(rhs.data)) {
      return Instance::new_float(
          std::fmod(static_cast<FloatT>(lhs), get<FloatT>(rhs.data)));
    }
    throw "unexpected operation";
  }

  Instance operator()(const FloatT lhs) {
    if (holds<IntT>(rhs.data)) {
      return Instance::new_float(
          std::fmod(lhs, static_cast<FloatT>(get<IntT>(rhs.data))));
    } else if (holds<FloatT>(rhs.data)) {
      return Instance::new_float(std::fmod(lhs, get<FloatT>(rhs.data)));
    }
    throw "unexpected operation";
  }

  // str % str strips lhs of trailing chars in rhs
  Instance operator()(const StringT lhs) {
    if (!holds<StringT>(rhs.data)) {
      throw "unexpected operation";
    }
    StringT new_str = lhs;
    StringT rem = get<StringT>(rhs.data);

    while (!new_str.empty()) {
      bool did_del = false;
      for (char c : rem) {
        if (new_str.at(new_str.size() - 1) == c) {
          new_str.erase(new_str.size() - 1);
          did_del = true;
        }
      }
      if (!did_del) {
        break;
      }
    }
    return Instance::new_string(new_str);
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
