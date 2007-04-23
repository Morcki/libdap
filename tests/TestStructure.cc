
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.
 
// (c) COPYRIGHT URI/MIT 1995-1999
// Please read the full copyright statement in the file COPYRIGHT_URI.
//
// Authors:
//      jhrg,jimg       James Gallagher <jgallagher@gso.uri.edu>

// Implementation for the class TestStructure. See TestByte.cc
//
// jhrg 1/12/95

#include "config.h"
#include "TestStructure.h"

void
TestStructure::_duplicate(const TestStructure &ts)
{
    d_series_values = ts.d_series_values;
}

BaseType *
TestStructure::ptr_duplicate()
{
    return new TestStructure(*this);
}

TestStructure::TestStructure(const TestStructure &rhs) : Structure(rhs), TestCommon(rhs)
{
    _duplicate(rhs);
}

TestStructure &
TestStructure::operator=(const TestStructure &rhs)
{
    if (this == &rhs)
	return *this;

    dynamic_cast<Structure &>(*this) = rhs; // run Constructor=

    _duplicate(rhs);

    return *this;
}

TestStructure::TestStructure(const string &n, const string &ds)
    : Structure(n, ds), d_series_values(false)
{
}

TestStructure::~TestStructure()
{
}

// For this `Test' class, run the read mfunc for each of variables which
// comprise the structure. 

bool
TestStructure::read(const string &dataset)
{
    if (read_p())
	return true;

    for (Vars_iter i = var_begin(); i != var_end(); i++)
    {
	if (!(*i)->read(dataset))
	{
	    return false;
	}
    }

    set_read_p(true);

    return true;
}

void
TestStructure::set_series_values(bool sv)
{
    Vars_iter i = var_begin();
    while (i != var_end()) {
        dynamic_cast<TestCommon&>(*(*i)).set_series_values(sv);
        ++i;
    }
    
    d_series_values = sv;
}
