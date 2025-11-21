#pragma once
#include "../../typing/kind.h"
#include "../../typing/subtype.h"

#include <memory>
using std::get;
using std::make_shared;
using std::shared_ptr;

AtomV _mul(const AtomVAccess& la, const AtomVAccess& ra) {
	// assumes RHS is a subtype of LHS (will get called twice)
	if (la.get_kind() != TYPE_KIND or ra.get_kind() != TYPE_KIND) {
		throw std::runtime_error("mul not supported for non-type-kinds");
	}

	switch (la.get_type()) {
	case FLOAT_TYPE: {
		if (!type_subtype(ra.get_type(), FLOAT_TYPE)) {
			throw NotImplementedT();
		}
		double lval = get<shared_ptr<Float>>(la.inner)->get_value();
		switch (ra.get_type()) {
		case FLOAT_TYPE: {
			double rval = get<shared_ptr<Float>>(ra.inner)->get_value();
			return make_shared<Float>(lval * rval);
		}
		case INT64_TYPE: {
			double rval = static_cast<double>(get<shared_ptr<Int64>>(ra.inner)->get_value());
			return make_shared<Float>(lval * rval);
		}
		case BOOL_TYPE: {
			double rval = static_cast<double>(get<shared_ptr<Bool>>(ra.inner)->get_state() ? 1 : 0);
			return make_shared<Float>(lval * rval);
		}
		default:
			throw NotImplementedT();
		}
	}
	case INT64_TYPE: {
		if (!type_subtype(ra.get_type(), INT64_TYPE)) {
			throw NotImplementedT();
		}
		int64_t lval = get<shared_ptr<Int64>>(la.inner)->get_value();
		switch (ra.get_type()) {
		case INT64_TYPE: {
			int64_t rval = get<shared_ptr<Int64>>(ra.inner)->get_value();
			return make_shared<Int64>(lval * rval);
		}
		case BOOL_TYPE: {
			int64_t rval = get<shared_ptr<Bool>>(ra.inner)->get_state() ? 1 : 0;
			return make_shared<Int64>(lval * rval);
		}
		default:
			throw NotImplementedT();
		}
	}
	case BOOL_TYPE: {
		if (!type_subtype(ra.get_type(), BOOL_TYPE)) {
			throw NotImplementedT();
		}
		bool lval = get<shared_ptr<Bool>>(la.inner)->get_state();
		switch (ra.get_type()) {
		case BOOL_TYPE: {
			bool rval = get<shared_ptr<Bool>>(ra.inner)->get_state();
			return make_shared<Int64>((lval ? 1 : 0) * (rval ? 1 : 0));
		}
		default:
			throw NotImplementedT();
		}
	}
	default:
		throw NotImplementedT();
	}
}
AtomV mul(const AtomV& lhs, const AtomV& rhs) {
	const AtomVAccess la = AtomVAccess(lhs);
	const AtomVAccess ra = AtomVAccess(rhs);

	if (la.get_kind() != TYPE_KIND or ra.get_kind() != TYPE_KIND) {
		throw std::runtime_error("mul not supported for non-type-kinds");
	}
	try {
		return _mul(la, ra);
	} catch (NotImplementedT) { return _mul(ra, la); }
}