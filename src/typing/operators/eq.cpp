#pragma once
#include "../atom.h"


bool eq(const Atom& left, const Atom& right) {
    auto left_acc = AtomAccess(left);
    auto right_acc = AtomAccess(right);

    if (left_acc.get_kind() != right_acc.get_kind()) {
        return false;
    }
}