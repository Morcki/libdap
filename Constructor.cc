
// -*- mode: c++; c-basic-offset:4 -*-

// (c) COPYRIGHT URI/MIT 1995-1999
// Please read the full copyright statement in the file COPYRIGHT.
//
// Authors:
//      jhrg,jimg       James Gallagher <jgallagher@gso.uri.edu>

#ifdef _GNUG_
#pragma implementation
#endif

#include "config_dap.h"

#include <string>
#include <strstream>

#include "Constructor.h"

#include "debug.h"
#include "Error.h"
#include "InternalErr.h"

#ifdef TRACE_NEW
#include "trace_new.h"
#endif

using std::endl;

// Private member functions

void
Constructor::_duplicate(const Constructor &s)
{
}

// Public member functions

Constructor::Constructor(const string &n, const Type &t) 
    : BaseType(n, t)
{
}

Constructor::Constructor(const Constructor &rhs) : BaseType(rhs)
{
}

Constructor::~Constructor()
{
}

Constructor &
Constructor::operator=(const Constructor &rhs)
{
    if (this == &rhs)
	return *this;

    dynamic_cast<BaseType &>(*this) = rhs; // run BaseType=

    _duplicate(rhs);

    return *this;
}

    /** True if the instance can be flattened and printed as a single table
	of values. For Arrays and Grids this is always false. For Structures
	and Sequences the conditions are more complex. The implementation
	provided by this class always returns false. Other classes should
	override this implementation.

	@brief Check to see whether this variable can be printed simply.
	@return True if the instance can be printed as a single table of
	values, false otherwise. */
bool
Constructor::is_linear()
{
    return false;
}

// $Log: Constructor.cc,v $
// Revision 1.5  2003/01/10 19:46:40  jimg
// Merged with code tagged release-3-2-10 on the release-3-2 branch. In many
// cases files were added on that branch (so they appear on the trunk for
// the first time).
//
// Revision 1.1.2.3  2002/08/08 06:54:56  jimg
// Changes for thread-safety. In many cases I found ugly places at the
// tops of files while looking for globals, et c., and I fixed them up
// (hopefully making them easier to read, ...). Only the files RCReader.cc
// and usage.cc actually use pthreads synchronization functions. In other
// cases I removed static objects where they were used for supposed
// improvements in efficiency which had never actually been verifiied (and
// which looked dubious).
//
// Revision 1.4  2002/06/18 15:36:24  tom
// Moved comments and edited to accommodate doxygen documentation-generator.
//
// Revision 1.3  2001/09/28 17:50:07  jimg
// Merged with 3.2.7.
//
// Revision 1.1.2.2  2001/09/25 20:35:28  jimg
// Added a default definition for is_linear().
//
// Revision 1.2  2001/06/15 23:49:01  jimg
// Merged with release-3-2-4.
//
// Revision 1.1.2.1  2001/06/05 16:04:39  jimg
// Created.
//