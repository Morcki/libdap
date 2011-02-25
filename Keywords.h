// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2011 OPeNDAP, Inc.
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

#ifndef KEYWORDS_H_
#define KEYWORDS_H_

#include <string>
#include <set>
#include <map>
#include <list>

using namespace std;

namespace libdap {

/**
 * Manage keywords for libdap. These are passed in to the library using the
 * constraint expression - in fact they are an extension of the CE and this
 * class implements the parsing needed to remove them from the CE so that
 * the ConstraintExpression evaluator can parse it (because the keywords are
 * not identifiers in the DDS, they will cause a parse error.
 *
 * @note The keywords are used to specify the DAP version(s) that the client
 * can understand
 */
class Keywords {
public:
    enum keyword_kind {
	dap_version, none
    };

    // convenience types
    typedef string keyword;
    typedef string keyword_value;
    typedef pair<keyword_kind, keyword_value> kind_value_t;
    typedef pair<keyword, keyword_value> keyword_value_t;

private:
    // Note that the known keywords are indexed by keyword while the parsed
    // keywords are indexed b keyword_kind. And we can have only one instance
    // of each index - so there can be only one keyword,keyword_value pair
    // for Keywords::dap_protocol, e.g.

    /// Holds the keyword_kind and value of the keywords passed in the CE
    map<keyword_kind, pair<string, string> > d_parsed_keywords;

    /// Holds all of the keywords and their keyword_kind and value
    map<string, pair<keyword_kind, string> > d_known_keywords;

    void m_init();
    // Not needed w/o pointers Keyword &clone(const Keyword &rhs);
    void m_insert_tuple(const string &k, keyword_kind kind, const string &v);

public:
    Keywords();
    virtual ~Keywords();

    virtual string parse_keywords(const string &ce);
    virtual void add_keyword(const string &kw);

    // Is this keyword in the dictionary?
    virtual bool is_known_keyword(const string &w) const;

    // Get a list of all of the keywords parsed
    virtual list<string> get_keywords() const;
    // Has a keyword of a particular kind been parsed
    virtual bool has_keyword_kind(const keyword_kind &kind) const;

    // Get the parsed keyword (and it's dictionary value) of a particular kind
    virtual string get_kind_value(const keyword_kind &kind) const;
    virtual string get_kind_keyword(const keyword_kind &kind) const;

    // Look in the dictionary and get the value and kind for a particular keyword
    virtual string get_keyword_value(const keyword &k) const;
    virtual keyword_kind get_keyword_kind(const keyword &k) const;
};

}

#endif /* KEYWORDS_H_ */
