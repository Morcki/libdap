
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1996, California Institute of Technology.
// ALL RIGHTS RESERVED.   U.S. Government Sponsorship acknowledged.
//
// Please read the full copyright notice in the file COPYRIGH
// in this directory.
//
// Author: Todd Karakashian, NASA/Jet Propulsion Laboratory
//         Todd.K.Karakashian@jpl.nasa.gov
//
// $RCSfile: escaping.cc,v $ - Miscellaneous routines for DODS HDF server
//
// $Log: escaping.cc,v $
// Revision 1.4  1997/02/14 04:18:10  jimg
// Added allowable and escape parameters to id2dods and dods2id so that the
// builtin regexs can be overridden if needed.
// Switched to the `fast compile' mode for the Regex objects.
//
// Revision 1.3  1997/02/14 02:24:44  jimg
// Removed reliance on the dods-hdf code.
// Introduced a const int MAXSTR with value 256. This matches the length of
// ID_MAX in the parser.h header (which I did not include since it defines
// a lot of software that is irrelevant to this module).
//
// Revision 1.2  1997/02/14 02:18:16  jimg
// Added to DODS core
//
// Revision 1.2  1996/10/07 21:15:17  todd
// Changes escape character to % from _.
//
// Revision 1.1  1996/09/24 22:38:16  todd
// Initial revision
//
//
/////////////////////////////////////////////////////////////////////////////

// These two routines are for escaping/unescaping strings that are identifiers
// in DODS
// id2dods() -- escape (using WWW hex codes) non-allowable characters in a
// DODS identifier
// dods2id() -- given an WWW hexcode escaped identifier, restore it
// 
// These two routines are for escaping/unescaping strings storing attribute
// values.  They use traditional octal escapes (\nnn) because they are
// intended to be viewed by a user
// escattr() -- escape (using traditional octal backslash) non-allowable
// characters in the value of a DODS attribute
// unescattr() -- given an octally escaped string, restore it
// 
// These are routines used by the above, not intended to be called directly:
// 
// hexstring()
// unhexstring()
// octstring()
// unoctstring()
// 
// -Todd

#include <ctype.h>
#include <strstream.h>
#include <iomanip.h>
#include <String.h>

#if 0
#include <mfhdf.h>
#include <hdfclass.h>
#include "HDFStructure.h"
#include "HDFArray.h"
#include "hdfutil.h"
#include "dhdferr.h"
#endif

const int MAXSTR = 256;

String hexstring(int val) {
    static char buf[MAXSTR];

    ostrstream(buf,MAXSTR) << hex << setw(2) << setfill('0') <<
	val << ends;

    return (String)buf;
}

char unhexstring(String s) {
    int val;
    static char buf[MAXSTR];

    strcpy(buf,(const char *)s);
    istrstream(buf,MAXSTR) >> hex >> val;

    return (char)val;
}

String octstring(int val) {
    static char buf[MAXSTR];

    ostrstream(buf,MAXSTR) << oct << setw(3) << setfill('0') <<
	val << ends;

    return (String)buf;
}

char unoctstring(String s) {
    int val;
    static char buf[MAXSTR];

    strcpy(buf,(const char *)s);
    istrstream(buf,MAXSTR) >> oct >> val;

    return (char)val;
}


// replace characters that are not allowed in DODS identifiers
String id2dods(String s, const String allowable = (char *)0) {
    static Regex badregx("[^0-9a-zA-Z_%]", 1);
    static const char ESC = '%';

    if (allowable) {
	Regex badregx2(allowable, 1);

	int index;
	while ((index = s.index(badregx2)) >= 0)
	    s.at(index,1) = ESC + hexstring(toascii(s[index]));
    }
    else {
	int index;
	while ((index = s.index(badregx)) >= 0)
	    s.at(index,1) = ESC + hexstring(toascii(s[index]));
    }

    if (isdigit(s[0]))
	s.before(0) = '_';

    return s;
}

String dods2id(String s, const String escape = (char *)0) {
    static Regex escregx("%[0-7][0-9a-fA-F]", 1);

    if (escape) {
	Regex escregx2(escape, 1);
	int index;
	while ((index = s.index(escregx2)) >= 0)
	    s.at(index,3) = unhexstring(s.at(index+1,2));
    }
    else {
	int index;
	while ((index = s.index(escregx)) >= 0)
	    s.at(index,3) = unhexstring(s.at(index+1,2));
    }
    return s;
}

// Escape non-printable characters and quotes from an HDF attribute
String escattr(String s) {
    static Regex nonprintable = "[^ !-~]";
    const char ESC = '\\';
    const char QUOTE = '\"';
    const String ESCQUOTE = (String)ESC + (String)QUOTE;

    // escape non-printing characters with octal escape
    int index = 0;
    while ( (index = s.index(nonprintable)) >= 0)
	s.at(index,1) = ESC + octstring(toascii(s[index]));

    // escape " with backslash
    index = 0;
    while ( (index = s.index(QUOTE,index)) >= 0) {
	s.at(index,1) = ESCQUOTE;
	index += ESCQUOTE.length();
    }

    return s;
}

// Un-escape special characters, quotes and backslashes from an HDF attribute.
// Note: A regex to match one \ must be defined as
//          Regex foo = "\\\\";
//       because both C++ strings and libg++ regex's also employ \ as
//       an escape character!
String unescattr(String s) {
    static Regex escregx = "\\\\[01][0-7][0-7]";  // matches 4 characters
    static Regex escquoteregex = "[^\\\\]\\\\\"";  // matches 3 characters
    static Regex escescregex = "\\\\\\\\";      // matches 2 characters
    const char ESC = '\\';
    const char QUOTE = '\"';
    const String ESCQUOTE = (String)ESC + (String)QUOTE;

    // unescape any octal-escaped ASCII characters
    int index = 0;
    while ( (index = s.index(escregx,index)) >= 0) {
	s.at(index,4) = unoctstring(s.at(index+1,3));
	index++;
    }

    // unescape any escaped quotes
    index = 0;
    while ( (index = s.index(escquoteregex,index)) >= 0) {
	s.at(index+1,2) = QUOTE;
	index++;
    }

    // unescape any escaped backslashes
    index = 0;
    while ( (index = s.index(escescregex,index)) >= 0) {
	s.at(index,2) = ESC;
	index++;
    }

    return s;
}