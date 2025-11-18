#pragma once
enum Brand { FUNCTION, TYPE };

// TYPE brand must also implement TypeKind
enum TypeKind { OBJECT, BOOL, VOID, INT64 };