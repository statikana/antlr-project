#pragma once
#include "koda_types.cpp"
#include <iostream>
#include <map>
#include <memory>
#include <variant>
#include <vector>

// global static references to specific types
// all types must be defined in koda_types.h
// initialize global type singletons to real instances so .type is never null.
auto g_VoidType = std::make_shared<VoidType>();
auto g_BoolType = std::make_shared<BoolType>();
auto g_IntType = std::make_shared<IntType>();
auto g_FloatType = std::make_shared<FloatType>();
auto g_StringType = std::make_shared<StringType>();
auto g_ArrayType = std::make_shared<ArrayType>();
// no g_StructType: each unique struct points to a specific struct

class Instance;

// // this is the only type which doesn't have an exact c++ mirror
struct StructData {
public:
  StructData(std::map<std::string, std::shared_ptr<Instance>> fields) {
    this->fields = std::move(fields);
  }
  std::map<std::string, std::shared_ptr<Instance>> fields;
};

using ArrayData = std::vector<std::shared_ptr<Instance>>;
using ArrayDataPtr = std::shared_ptr<ArrayData>;
using StructDataPtr = std::shared_ptr<StructData>;

using VoidT = std::monostate;
using BoolT = bool;
using IntT = long int;
using FloatT = long double;
using StringT = std::string;

using Data = std::variant<ArrayDataPtr, StructDataPtr, VoidT, BoolT, IntT,
                          FloatT, StringT>;

template <typename T> bool holds(const Data &var) {
  return std::holds_alternative<T>(var);
}

template <typename T> T get(const Data &var) { return std::get<T>(var); }

class Instance {
public:
  Data data;
  std::shared_ptr<Type> type;

  // does not define an implicit constuctor
  // to create void, explicitly call Instance::new_void() (no arguments)
  Instance(Data data, std::shared_ptr<Type> type) {
    this->data = std::move(data);
    this->type = std::move(type);
  }

  // helper functions to create types
  static Instance new_void() { return Instance(std::monostate(), g_VoidType); }

  static Instance new_bool(bool value) { return Instance(value, g_BoolType); }

  static Instance new_int(long int value) { return Instance(value, g_IntType); }

  static Instance new_float(long double value) {
    return Instance(value, g_FloatType);
  }

  static Instance new_string(std::string value) {
    return Instance(value, g_StringType);
  }

  static Instance new_array(ArrayData value) {
    return Instance(std::make_shared<ArrayData>(std::move(value)), g_ArrayType);
  }

  static Instance new_struct(StructData value, std::shared_ptr<Type> s_type) {
    return Instance(std::make_shared<StructData>(std::move(value)), s_type);
  }

  std::string to_text() const {
    switch (type->get_kind()) {
    case VOID:
      return "void";
    case BOOL:
      return get<bool>(data) ? "true" : "false";
    case INT:
      return std::to_string(get<long int>(data));
    case FLOAT: {
      StringT s = std::to_string(get<long double>(data));
      // remove trailing 0s
      s.erase(s.find_last_not_of('0') + 1);
      return s;
    }
    case STRING:
      return get<std::string>(data);
    case ARRAY: {
      std::string str = "[";
      ArrayDataPtr raw = get<ArrayDataPtr>(data);

      for (size_t i = 0; i < raw->size() - 1; i++) {
        str.append(raw->at(i)->to_text() + ", ");
      }
      str.append(raw->at(raw->size() - 1)->to_text());
      return str;
    }
    default:
      throw std::runtime_error("to_text undefined for type " +
                               type->get_name());
    }
  }
};