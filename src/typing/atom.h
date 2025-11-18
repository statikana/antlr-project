#pragma once
#include "./object.h"
#include "./std/bool.h"
#include "./std/int64.h"
#include "./std/void.h"

#include <utility>
#include <variant>

using std::shared_ptr;

// variant responsible for holding
// any object in the type system.
// keep updated!
typedef std::variant<shared_ptr<Object>, shared_ptr<Bool>, shared_ptr<Void>, shared_ptr<Int64>> Atom;
