#pragma once

#include <string>


enum TypeKind {
    VOID, BOOL, INT, FLOAT, STRING,
};

class Type {
    public:
        virtual TypeKind get_kind() const = 0;
        virtual std::string get_name() const = 0;
};

class VoidType : public Type {
    public:
        TypeKind get_kind() const override { return TypeKind::VOID; }
        std::string get_name() const override { return "void"; }
};

class BoolType : public Type {
    public:
        TypeKind get_kind() const override { return TypeKind::BOOL; }
        std::string get_name() const override { return "boolean"; }
};

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
