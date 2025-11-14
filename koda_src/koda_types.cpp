#pragma once
#include <string>


enum TypeKind {
    VOID, BOOL, INT, FLOAT, STRING, ARRAY, STRUCT
};

class Type {
    public:
        virtual TypeKind get_kind() const = 0;
        virtual std::string get_name() const = 0;
};

// std::monostate: 1
class VoidType : public Type {
    public:
        TypeKind get_kind() const override { return TypeKind::VOID; }
        std::string get_name() const override { return "void"; }
};

// bool: 1
class BoolType : public Type {
    public:
        TypeKind get_kind() const override { return TypeKind::BOOL; }
        std::string get_name() const override { return "boolean"; }
};

// long int
class IntType : public Type {
    public:
        TypeKind get_kind() const override { return TypeKind::INT; }
        std::string get_name() const override { return "integer"; }
};

class FloatType : public Type {
    public:
        TypeKind get_kind() const override { return TypeKind::FLOAT; }
        std::string get_name() const override { return "float"; }
};

class StringType : public Type {
    public:
        TypeKind get_kind() const override { return TypeKind::STRING; }
        std::string get_name() const override { return "string"; }
};
class ArrayType : public Type {
    public:
        TypeKind get_kind() const override { return TypeKind::ARRAY; }
        std::string get_name() const override { return "array"; }
};
class StructType : public Type {
    public:
        TypeKind get_kind() const override { return TypeKind::STRUCT; }
        std::string get_name() const override { return name; }

        std::string name;
        std::map<std::string, std::shared_ptr<Type>> field_types;
        // methods...
};