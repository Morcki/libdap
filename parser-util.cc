
// (c) COPYRIGHT URI/MIT 1995-1999
// Please read the full copyright statement in the file COPYRIGHT.
//
// Authors:
//      jhrg,jimg       James Gallagher (jgallagher@gso.uri.edu)

// These functions are utility functions used by the various DAP parsers (the
// DAS, DDS and constraint expression parsers). 
// jhrg 9/7/95

// $Log: parser-util.cc,v $
// Revision 1.16  1999/04/29 02:29:37  jimg
// Merge of no-gnu branch
//
// Revision 1.15  1999/04/22 22:31:11  jimg
// Comments
//
// Revision 1.14  1999/03/29 17:35:50  jimg
// Fixed (I hope) a bug in check_float{32,64} where 0.0 did not check out as a
// valid floating point number. Note that the DODS_{FLT,DBL}_{MIN,MAX}
// constants are the absolute values of the bigest and smallest numbers
// representable, unlike the similar constants for integer types.
//
// Revision 1.13  1999/03/24 23:29:35  jimg
// Added support for the new Int16, UInt16 and Float32 types.
// Removed unused error printing functions.
//
// Revision 1.12.6.2  1999/02/05 09:32:36  jimg
// Fixed __unused__ so that it not longer clashes with Red Hat 5.2 inlined
// math code. 
//
// Revision 1.12.6.1  1999/02/02 21:57:08  jimg
// String to string version
//
// Revision 1.12  1998/03/19 23:28:42  jimg
// Removed old code (that was surrounded by #if 0 ... #endif).
//
// Revision 1.11  1998/02/05 20:14:05  jimg
// DODS now compiles with gcc 2.8.x
//
// Revision 1.10  1997/02/28 01:20:51  jimg
// Removed `parse error' message from parse_error() function.
//
// Revision 1.9  1996/10/28 23:05:54  jimg
// Fixed tests in check_uint().
// NB: strtol() does not check for overflow on SunOS.
//
// Revision 1.8  1996/10/28 18:53:13  jimg
// Added functions to test unsigned integers.
//
// Revision 1.7  1996/08/13 20:43:38  jimg
// Added not_used to definition of char rcsid[].
// Added a new parse_error function that builds an Error object and returns it
// instead of printing to stderr.
// Added versions of check_*() that take parser_arg and a context string. These
// call the new parse_error() function.
//
// Revision 1.6  1996/06/14 23:30:33  jimg
// Added `<< ends' to the lines where ostrstream objects are used (without this
// there is no null added to the end of the streams).
//
// Revision 1.5  1996/06/08 00:12:25  jimg
// Improved error messages in some of the type checking functions.
//
// Revision 1.4  1996/05/31 23:31:05  jimg
// Updated copyright notice.
//
// Revision 1.3  1996/05/06 18:33:24  jimg
// Replaced calls to atoi with calls to strtol.
//
// Revision 1.2  1996/05/04 00:07:33  jimg
// Fixed a bug where Float attributes with the value 0.0 were considered `bad
// values'.
//
// Revision 1.1  1996/04/04 22:12:19  jimg
// Added.

#include "config_dap.h"

static char rcsid[] not_used = {"$Id: parser-util.cc,v 1.16 1999/04/29 02:29:37 jimg Exp $"};

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <iostream>
#ifdef __GNUG__
#include <strstream>
#else
#include <sstream>
#endif

#include "parser.h"		// defines constants such as ID_MAX
#include "dods-limits.h"

void 
parse_error(const char *s, const int line_num)
{
    assert(s);

    cerr << "Parse error (line: " << line_num << "):" << endl
	 << s << endl;
}

void
parse_error(parser_arg *arg, const char *msg, const int line_num,
	    const char *context)
{ 
    assert(arg);
    assert(msg);

    arg->set_status(FALSE);

    ostrstream oss;

    if (line_num != 0)
	oss << "Error parsing the text on line " << line_num << ":" << endl;

    if (context)
	oss << msg << endl << context << ends;
    else
	oss << msg << ends;

    arg->set_error(new Error(unknown_error, oss.str()));
    oss.freeze(0);
}

void
save_str(char *dst, const char *src, const int line_num)
{
    strncpy(dst, src, ID_MAX);
    dst[ID_MAX-1] = '\0';		/* in case ... */
    if (strlen(src) >= ID_MAX) 
	cerr << "line: " << line_num << "`" << src << "' truncated to `"
             << dst << "'" << endl;
}

void
save_str(string &dst, const char *src, const int)
{
    dst = src;
}

int
check_byte(const char *val, const int line)
{
    char *ptr;
    long v = strtol(val, &ptr, 0);

    if (v == 0 && val == ptr) {
	parse_error("Not decodable to an integer value", line);
	return FALSE;
    }

    if (v > DODS_CHAR_MAX || v < DODS_CHAR_MIN) {
	parse_error("Not a byte value", line);
	return FALSE;
    }

    return TRUE;
}

// This version of check_int will pass base 8, 10 and 16 numbers when they
// use the ANSI standard for string representation of those number bases.

int
check_int16(const char *val, const int line)
{
    char *ptr;
    long v = strtol(val, &ptr, 0); // `0' --> use val to determine base

    if (v == 0 && val == ptr) {
	ostrstream oss;
	oss << "`" << val << "' cannot be decoded as an integer value." 
	    << ends;

	parse_error(oss.str(), line);
	oss.freeze(0);

	return FALSE;
    }

    // Don't use the constant from limits.h, use the ones in dods-limits.h
    if (v > DODS_SHRT_MAX || v < DODS_SHRT_MIN) { 
	ostrstream oss;

	oss << "`" << val << "' is not a 16-bit integer value value." << endl
	    << "It must be between " << DODS_SHRT_MIN << " and "
	    << DODS_SHRT_MAX << "." << ends;
	parse_error(oss.str(), line);
	oss.freeze(0);

	return FALSE;
    }

    return TRUE;
}

int
check_uint16(const char *val, const int line)
{
    char *ptr;
    unsigned long v = strtol(val, &ptr, 0); 

    if (v == 0 && val == ptr) {
	ostrstream oss;
	oss << "`" << val << "' cannot be decoded as an integer value." 
	    << ends;
	parse_error(oss.str(), line);
	oss.rdbuf()->freeze(0);
	return FALSE;
    }

    if (v > DODS_USHRT_MAX) { 
	ostrstream oss;
	oss << "`" << val << "' is not a 16-bit integer value value." << endl
	    << "It must be less than or equal to " << DODS_USHRT_MAX << "."
	    << ends;
	parse_error(oss.str(), line);
	oss.freeze(0);
	return FALSE;
    }

    return TRUE;
}

int
check_int32(const char *val, const int line)
{
    char *ptr;
    long v = strtol(val, &ptr, 0); // `0' --> use val to determine base

    if (v == 0 && val == ptr) {
	ostrstream oss;
	oss << "`" << val << "' cannot be decoded as an integer value." 
	    << ends;

	parse_error(oss.str(), line);
	oss.freeze(0);

	return FALSE;
    }

    if (v > DODS_INT_MAX || v < DODS_INT_MIN) { 
	ostrstream oss;
	oss << "`" << val << "' is not a 32-bit integer value value." << endl
	    << "It must be between " << DODS_INT_MIN << " and "
	    << DODS_INT_MAX << "." << ends;

	parse_error(oss.str(), line);
	oss.freeze(0);

	return FALSE;
    }

    return TRUE;
}

int
check_uint32(const char *val, const int line)
{
    char *ptr;
    unsigned long v = strtol(val, &ptr, 0);

    if (v == 0 && val == ptr) {
	ostrstream oss;
	oss << "`" << val << "' cannot be decoded as an integer value." 
	    << ends;
	parse_error(oss.str(), line);
	oss.freeze(0);
	return FALSE;
    }

    return TRUE;
}

// This function does not test for numbers that are smaller than
// DODS_FLT_MIN. That is hard to do without eliminating valid numbers such as
// 0.0. Maybe the solution is to test for 0.0 specially? 4/12/99 jhrg 

int
check_float32(const char *val, const int num)
{
    char *ptr;
    double v = strtod(val, &ptr);

    if (v == 0.0 && val == ptr) {
	parse_error("Not decodable to a 32-bit float value", num);
	return FALSE;
    }

    static double range = fabs(log10(DODS_FLT_MAX));
    if (v != 0.0 && fabs(log10(fabs(v))) > range) { 
	ostrstream oss;

	oss << "`" << val << "' is not a 32 bit floating point value value." 
	    << endl
	    << "It must be between (+/-)" << DODS_FLT_MAX << " and (+/-)" 
	    << DODS_FLT_MIN << "." << ends;
	parse_error(oss.str(), num);
	oss.freeze(0);

	return FALSE;
    }

    return TRUE;
}

int
check_float64(const char *val, const int num)
{
    char *ptr;
    double v = strtod(val, &ptr);

    if (v == 0.0 && val == ptr) {
	parse_error("Not decodable to a 64-bit float value", num);
	return FALSE;
    }

    static double range = fabs(log10(DODS_DBL_MAX));
    if (v != 0.0 && fabs(log10(fabs(v))) > range) { 
	ostrstream oss;

	oss << "`" << val << "' is not a 64 bit floating point value value." 
	    << endl
	    << "It must be between (+/-)" << DODS_DBL_MAX << " and (+/-)" 
	    << DODS_DBL_MIN << "." << ends;
	parse_error(oss.str(), num);
	oss.freeze(0);

	return FALSE;
    }

    return TRUE;
}

/*
  Maybe someday we will really check the Urls to see if they are valid...
*/

int
check_url(const char *, const int)
{
    return TRUE;
}
