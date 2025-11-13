#pragma once
#include "koda_types.cpp"
#include <variant>
#include <memory>
#include <map>
#include <iostream>


// global static references to specific types
// all types must be defined in koda_types.h
// initialize global type singletons to real instances so .type is never null.
auto g_VoidType = std::make_shared<VoidType>();
auto g_BoolType = std::make_shared<BoolType>();
auto g_IntType = std::make_shared<IntType>();
auto g_FloatType = std::make_shared<FloatType>();
auto g_StringType = std::make_shared<StringType>();

class Instance;

using Data = std::variant<
    std::monostate,     // void
    bool,               // bool
    int,                // int
    long double,        // float
    std::string         // string
>;


class Instance {
    public:
        Data data;
        std::shared_ptr<Type> type;

        // does not define an implicit constuctor
        // to create void, explicitly call Instance::new_void() (no arguments)
        Instance(Data data, std::shared_ptr<Type> type) {
            this->data = data;
            this->type = type;
        }

        // helper functions to create types
        static Instance new_void() 
            { return Instance(std::monostate(), g_VoidType); }

        static Instance new_bool(bool value)         
            { return Instance(value, g_BoolType); }
            
        static Instance new_int(int value)
        
            { return Instance(value, g_IntType); }

        static Instance new_float(long double value)  
            { return Instance(value, g_FloatType); }

        static Instance new_string(std::string value) 
            { return Instance(value, g_StringType); }

        std::string to_text() const {
            switch (type->get_kind()) {
                case VOID:
                    return "void";
                case BOOL:
                    return std::get<int>(data) == 1 ? "true" : "false";
                case INT:
                    return std::to_string(std::get<int>(data));
                case FLOAT:
                    return std::to_string(std::get<long double>(data));
                case STRING:
                    return std::get<std::string>(data);
                default:
                    throw "to_text undefined for type " + type->get_name();
            }
        }
};