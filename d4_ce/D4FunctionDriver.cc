// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2002,2003 OPeNDAP, Inc.
// Author: James Gallagher <jgallagher@opendap.org>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.

#include <cstdlib>

#include <string>
#include <sstream>
#include <iterator>

//#define DODS_DEBUG

#include "D4FunctionScanner.h"
#include "D4FunctionDriver.h"
#include "d4_function_parser.tab.hh"
#include "DMR.h"
#include "D4RValue.h"

#include "BaseType.h"

#include "escaping.h"
#include "debug.h"

namespace libdap {

bool D4FunctionDriver::parse(const std::string &expr)
{
	d_expr = expr;	// set for error messages. See the %initial-action section of .yy

	std::istringstream iss(expr);
	D4FunctionScanner *scanner = new D4FunctionScanner(iss);

	D4FunctionParser *parser = new D4FunctionParser(*scanner, *this /* driver */);

	if (trace_parsing()) {
		parser->set_debug_level(1);
		parser->set_debug_stream(std::cerr);
	}

	return parser->parse() == 0;
}

/**
 * Build and return a new RValue. Allocates the new D4RValue object.
 * The code tries first to find the id in the DMR - that is, it checks
 * first to see if it is a variable in the current dataset. If that
 * fails it will try to build an unsigned long long, a long long or
 * a double from the string (in that order). If that fails the code
 * converts the id into a string.
 *
 * @param id An identifier (really a string) parsed from the function
 * expression. May contain quotes.
 * @return Return a pointer to the new allocated D4RValue object.
 */
D4RValue *
D4FunctionDriver::build_rvalue(const std::string &id)
{
    BaseType *btp = 0;

    // Look for the id in the dataset first
    if (top_basetype()) {
        btp = top_basetype()->var(id);
    }
    else {
        btp = dmr()->root()->find_var(id);
    }

    if (btp)
        return new D4RValue(btp);

    // If the id is not a variable, try to turn it into a constant,
    // otherwise, its an error.
    char *end_ptr = 0;

    errno = 0;
    unsigned long long ull_val = strtoull(id.c_str(), &end_ptr, 0);
    if (end_ptr == '\0' && errno == 0)
    	return new D4RValue(ull_val);

    errno = 0;
    long long ll_val = strtoll(id.c_str(), &end_ptr, 0);
    if (end_ptr == '\0' && errno == 0)
    	return new D4RValue(ll_val);

    errno = 0;
    double d_val = strtod(id.c_str(), &end_ptr);
    if (end_ptr == '\0' && errno == 0)
    	return new D4RValue(d_val);

    return new D4RValue(www2id(id));
}

void
D4FunctionDriver::throw_not_found(const string &id, const string &ident)
{
    throw Error(d_expr + ": The variable " + id + " was not found in the dataset (" + ident + ").");
}

void
D4FunctionDriver::throw_not_array(const string &id, const string &ident)
{
	throw Error(d_expr + ": The variable '" + id + "' is not an Array variable (" + ident + ").");
}

// This method is called from the parser (see d4_ce_parser.yy, down in the code
// section). This will be called during the call to D4FunctionParser::parse(), that
// is inside D4FunctionDriver::parse(...)
void
D4FunctionDriver::error(const libdap::location &l, const std::string &m)
{
	std::cerr << l << ": " << m << std::endl;
}

#if 0
void
D4CEDriver::set_array_slices(const std::string &id, Array *a)
{
    // Test that the indexes and dimensions match in number
    if (d_indexes.size() != a->dimensions())
        throw Error("The index constraint for '" + id + "' does not match its rank.");

    Array::Dim_iter d = a->dim_begin();
    for (vector<index>::iterator i = d_indexes.begin(), e = d_indexes.end(); i != e; ++i) {
        if ((*i).stride > (unsigned long long)a->dimension_stop(d, false))
            throw Error("For '" + id + "', the index stride value is greater than the number of elements in the Array");
        if (!(*i).rest && ((*i).stop) > (unsigned long long)a->dimension_stop(d, false))
            throw Error("For '" + id + "', the index stop value is greater than the number of elements in the Array");

        D4Dimension *dim = a->dimension_D4dim(d);

        // In a DAP4 CE, specifying '[]' as an array dimension slice has two meanings.
        // It can mean 'all the elements' of the dimension or 'apply the slicing inherited
        // from the shared dimension'. The latter might be provide 'all the elements'
        // but regardless, the Array object must record the CE correctly.

        if (dim && (*i).empty) {
            a->add_constraint(d, dim);
        }
        else {
            a->add_constraint(d, (*i).start, (*i).stride, (*i).rest ? -1 : (*i).stop);
        }

        ++d;
    }

    d_indexes.clear();
}
#endif
#if 0
void
D4CEDriver::search_for_and_mark_arrays(BaseType *btp)
{
	DBG(cerr << "Entering D4CEDriver::search_for_and_mark_arrays...(" << btp->name() << ")" << endl);

	assert(btp->is_constructor_type());

	Constructor *ctor = static_cast<Constructor*>(btp);
	for (Constructor::Vars_iter i = ctor->var_begin(), e = ctor->var_end(); i != e; ++i) {
		switch ((*i)->type()) {
		case dods_array_c:
			DBG(cerr << "Found an array: " << (*i)->name() << endl);
			mark_array_variable(*i);
			break;
		case dods_structure_c:
		case dods_sequence_c:
			DBG(cerr << "Found a ctor: " << (*i)->name() << endl);
			search_for_and_mark_arrays(*i);
			break;
		default:
			break;
		}
	}
}

/**
 * When an identifier is used in a CE, is becomes part of the 'current projection,'
 * which means it is part of the set of variable to be sent back to the client. This
 * method sets a flag in the variable (send_p: send predicate) indicating that.
 *
 * @note This will check if the variable is an array and set it's slices accordingly
 * @param btp BaseType pointer to the variable. Must be non-null
 * @return The BaseType* to the variable; the send_p flag is set as a side effect.
 */
BaseType *
D4CEDriver::mark_variable(BaseType *btp)
{
    assert(btp);

    DBG(cerr << "In D4CEDriver::mark_variable... (" << btp->name() << "; " << btp->type_name() << ")" << endl);

    btp->set_send_p(true);

    if (btp->type() == dods_array_c ) {
    	mark_array_variable(btp);
    }

    // Test for Constructors and marks arrays they contain
	if (btp->is_constructor_type()) {
		search_for_and_mark_arrays(btp);
	}
	else if (btp->type() == dods_array_c && btp->var() && btp->var()->is_constructor_type()) {
		search_for_and_mark_arrays(btp->var());
	}

    // Now set the parent variables
    BaseType *parent = btp->get_parent();
    while (parent) {
        parent->BaseType::set_send_p(true); // Just set the parent using BaseType's impl.
        parent = parent->get_parent();
    }

    return btp;
}

/**
 * Add an array to the current projection with slicing. Calling this method will result
 * in the array being returned with anonymous dimensions.
 *
 * @note If id is an array that has shared dimensions and uses '[]' where a shared dimension
 * is found and if that shared dimension has been sliced, then the slice is used as the array's
 * slice for that dimension (there must be an easier way to explain that...)
 *
 * @param id
 * @return The BaseType* to the Array variable; the send_p and slicing information is
 * set as a side effect.
 */
BaseType *
D4CEDriver::mark_array_variable(BaseType *btp)
{
	assert(btp->type() == dods_array_c);

	Array *a = static_cast<Array*>(btp);

	// If an array appears in a CE without the slicing operators ([]) we still have to
	// call set_user_by_projected_var(true) for all of it's sdims for them to appear in
	// the CDMR.
	if (d_indexes.empty()) {
	    for (Array::Dim_iter d = a->dim_begin(), de = a->dim_end(); d != de; ++d) {
	        D4Dimension *dim = a->dimension_D4dim(d);
	        if (dim) {
	        	a->add_constraint(d, dim);
	        }
	    }
	}
    else {
        // Test that the indexes and dimensions match in number
        if (d_indexes.size() != a->dimensions())
            throw Error("The index constraint for '" + btp->name() + "' does not match its rank.");

        Array::Dim_iter d = a->dim_begin();
        for (vector<index>::iterator i = d_indexes.begin(), e = d_indexes.end(); i != e; ++i) {
            if ((*i).stride > (unsigned long long) a->dimension_stop(d, false))
                throw Error(
                        "For '" + btp->name()
                                + "', the index stride value is greater than the number of elements in the Array");
            if (!(*i).rest && ((*i).stop) > (unsigned long long) a->dimension_stop(d, false))
                throw Error(
                        "For '" + btp->name()
                                + "', the index stop value is greater than the number of elements in the Array");

            D4Dimension *dim = a->dimension_D4dim(d);

            // In a DAP4 CE, specifying '[]' as an array dimension slice has two meanings.
            // It can mean 'all the elements' of the dimension or 'apply the slicing inherited
            // from the shared dimension'. The latter might be provide 'all the elements'
            // but regardless, the Array object must record the CE correctly.

            if (dim && (*i).empty) {
                a->add_constraint(d, dim);  // calls set_used_by_projected_var(true) + more
            }
            else {
                a->add_constraint(d, (*i).start, (*i).stride, (*i).rest ? -1 : (*i).stop);
            }

            ++d;
        }

        d_indexes.clear();
    }

	return btp;
}

/**
 * Add an array to the current projection with slicing. Calling this method will result
 * in the array being returned with anonymous dimensions.
 * @param id
 * @return The BaseType* to the Array variable; the send_p and slicing information is
 * set as a side effect.
 */
D4Dimension *
D4CEDriver::slice_dimension(const std::string &id, const index &i)
{
    D4Dimension *dim = dmr()->root()->find_dim(id);

    if (i.stride > dim->size())
        throw Error("For '" + id + "', the index stride value is greater than the size of the dimension");
    if (!i.rest && (i.stop > dim->size() - 1))
        throw Error("For '" + id + "', the index stop value is greater than the size of the dimension");

    dim->set_constraint(i.start, i.stride, i.rest ? dim->size() - 1: i.stop);

    return dim;
}

D4CEDriver::index
D4CEDriver::make_index(const std::string &i)
{
	unsigned long long v = get_ull(i.c_str());
	return index(v, 1, v, false, false /*empty*/);
}

D4CEDriver::index
D4CEDriver::make_index(const std::string &i, const std::string &s, const std::string &e)
{
	return index(get_ull(i.c_str()), get_ull(s.c_str()), get_ull(e.c_str()), false, false /*empty*/);
}

D4CEDriver::index
D4CEDriver::make_index(const std::string &i, unsigned long long s, const std::string &e)
{
	return index(get_ull(i.c_str()), s, get_ull(e.c_str()), false, false /*empty*/);
}

D4CEDriver::index
D4CEDriver::make_index(const std::string &i, const std::string &s)
{
	return index(get_ull(i.c_str()), get_ull(s.c_str()), 0, true, false /*empty*/);
}

D4CEDriver::index
D4CEDriver::make_index(const std::string &i, unsigned long long s)
{
	return index(get_ull(i.c_str()), s, 0, true, false /*empty*/);
}
#endif



} /* namespace libdap */