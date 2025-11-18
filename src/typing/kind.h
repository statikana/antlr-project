#pragma once

#include <map>

enum Kind { FUNCTION_KIND, TYPE_KIND };
static std::map<Kind, std::string> kind_to_string = {
    {FUNCTION_KIND, "Function"},
    {TYPE_KIND, "Type"}
};

static std::map<std::string, Kind> string_to_kind = {
    {"Function", FUNCTION_KIND},
    {"Type", TYPE_KIND}
};


// TYPE_KIND Kind must also implement TypeKind
enum TypeKind { FUNCTION_TYPE, OBJECT_TYPE, BOOL_TYPE, VOID_TYPE, INT64_TYPE };
static std::map<std::string, TypeKind> string_to_type_kind = {
    {"Object", OBJECT_TYPE},
    {"Bool", BOOL_TYPE},
    {"Void", VOID_TYPE},
    {"Int64", INT64_TYPE}, {"int", INT64_TYPE}
};

static std::map<TypeKind, std::string> type_kind_to_string = {
    {OBJECT_TYPE, "Object"},
    {BOOL_TYPE, "Bool"},
    {VOID_TYPE, "Void"},
    {INT64_TYPE, "Int64"},
    {FUNCTION_TYPE, "Function"}
};