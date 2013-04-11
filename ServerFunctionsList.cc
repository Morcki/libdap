// ServerFunctionsList.cc

// This file is part of bes, A C++ back-end server implementation framework
// for the OPeNDAP Data Access Protocol.

// Copyright (c) 2013 OPeNDAP, Inc.
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

#include <iostream>
#include <algorithm>

#include <expr.h>

#include "ServerFunctionsList.h"
#include "debug.h"

using std::cerr;
using std::string;
using std::endl;
using namespace std;
using namespace libdap;

namespace libdap {

ServerFunctionsList *ServerFunctionsList::d_instance = 0 ;

ServerFunctionsList::ServerFunctionsList()
{
}

ServerFunctionsList::~ServerFunctionsList()
{
}


/**
 * Adds the passed ServerFunction pointer to the list of ServerFunctions using
 * the value of ServerFunction.getName() as the key in the list.
 *
 * @brief Adds the passed ServerFunction pointer to the list of ServerFunctions.
 * @param *func A pointer to the ServerFunction object to add to the ServerFunctionList.
 */
void ServerFunctionsList::add_function(ServerFunction *func )
{
	func_list.insert(std::make_pair(func->getName(),func));
}

#if 0

bool
ServerFunctionsList::add_function( string name, btp_func func )
{
    if (d_btp_func_list[name] == 0) {
        d_btp_func_list[name] = func;
        return true;
    }

    return false;
}


bool
ServerFunctionsList::add_function( string name, bool_func func )
{
    if (d_bool_func_list[name] == 0) {
        d_bool_func_list[name] = func;
        return true;
    }

    return false;
}

bool
ServerFunctionsList::add_function( string name, proj_func func )
{
    if (d_proj_func_list[name] == 0) {
        d_proj_func_list[name] = func;
        return true;
    }

    return false;
}
#endif

#if 0
void ServerFunctionsList::store_functions(ConstraintEvaluator &ce)
{
    if (d_btp_func_list.size() > 0) {
        map<string, btp_func>::iterator i = d_btp_func_list.begin();
        map<string, btp_func>::iterator e = d_btp_func_list.end();
        while (i != e) {
            ce.add_function((*i).first, (*i).second);
            ++i;
        }
    }

    if (d_bool_func_list.size() > 0) {
        map<string, bool_func>::iterator i = d_bool_func_list.begin();
        map<string, bool_func>::iterator e = d_bool_func_list.end();
        while (i != e) {
            ce.add_function((*i).first, (*i).second);
            ++i;
        }
    }

    if (d_proj_func_list.size() > 0) {
        map<string, proj_func>::iterator i = d_proj_func_list.begin();
        map<string, proj_func>::iterator e = d_proj_func_list.end();
        while (i != e) {
            ce.add_function((*i).first, (*i).second);
            ++i;
        }
    }
}
#endif

/**
 * Returns the first boolean function in the list whose key value matches the passed string name.
 * When a match is found the function returns true and sets returned value parameter *f to
 * the boolean function held by the ServerFunction object extracted from the list.
 *
 * Method:
 * Looks through the list of ServerFunctions and compares each function's key value (which
 * would be the value of SurverFunction.getName()) with the value of the string parameter
 * 'name'. When they match then the returned value parameter is set to the value returned
 * by ServerFunction.get_btp_func(). If the ServerFunction _is not_  a instance of a boolean
 * function then the return value will be 0 (null) and the search for matching function will continue.
 * If the ServerFunction _is_ a boolean function then the returned value will be non-zero and
 * the search will return true (it found the thing) and the returned value parameter *f will have
 * it's value set to the boolean function.
 *
 *  @brief Find a boolean function with a given name in the function list.
 *  @param name A string containing the name of the function to find.
 *  @param *f   A returned value parameter through which a point to the desired function is returned.
 *
 */
bool ServerFunctionsList::find_function(const std::string &name, bool_func *f) const
{
#if 0
    if (d_bool_func_list.empty())
        return false;

    map<string, bool_func>::const_iterator i = d_bool_func_list.begin();
    while(i != d_bool_func_list.end()) {
        if (name == (*i).first && (*f = (*i).second)) {
            return true;
        }
        ++i;
    }

    return false;
#endif

    if (func_list.empty())
        return false;

    std::pair <std::multimap<std::string,libdap::ServerFunction *>::const_iterator, std::multimap<std::string,libdap::ServerFunction *>::const_iterator> ret;
    ret = func_list.equal_range(name);
    for (std::multimap<std::string,libdap::ServerFunction *>::const_iterator it=ret.first; it!=ret.second; ++it) {
        if (name == it->first && (*f = it->second->get_bool_func())){
            return true;
        }
    }
    return false;

}



/**
 * Returns the first BaseType function in the list whose key value matches the passed string name.
 * When a match is found the function returns true and sets returned value parameter *f to
 * the BaseType function held by the ServerFunction object extracted from the list.
 *
 * Method:
 * Looks through the list of ServerFunctions and compares each function's key value (which
 * would be the value of SurverFunction.getName()) with the value of the string parameter
 * 'name'. When they match then the returned value parameter is set to the value returned
 * by ServerFunction.get_btp_func(). If the ServerFunction _is not_  a instance of a BaseType
 * function then the return value will be 0 (null) and the search for matching function will continue.
 * If the ServerFunction _is_ a BaseType function then the returned value will be non-zero and
 * the search will return true (it found the thing) and the returned value parameter *f will have
 * it's value set to the BaseType function.
 *
 *  @brief Find a BaseType function with a given name in the function list.
 *  @param name A string containing the name of the function to find.
 *  @param *f   A returned value parameter through which a point to the desired function is returned.
 *
 */
bool ServerFunctionsList::find_function(const string &name, btp_func *f) const
{

#if 0
    if (d_btp_func_list.empty())
        return false;

    map<string, btp_func>::const_iterator i = d_btp_func_list.begin();
    while(i != d_btp_func_list.end()) {
        if (name == (*i).first && (*f = (*i).second)) {
            return true;
        }
        ++i;
    }

    return false;
#endif

    if (func_list.empty())
        return false;

    std::pair <std::multimap<string,libdap::ServerFunction *>::const_iterator, std::multimap<string,libdap::ServerFunction *>::const_iterator> ret;
    ret = func_list.equal_range(name);
    for (std::multimap<string,libdap::ServerFunction *>::const_iterator it=ret.first; it!=ret.second; ++it) {
        if (name == it->first && (*f = it->second->get_btp_func())){
            return true;
        }
    }

    return false;



}

/**
 * Returns the first projection function in the list whose key value matches the passed string name.
 * When a match is found the function returns true and sets returned value parameter *f to
 * the projection function held by the ServerFunction object extracted from the list.
 *
 * Method:
 * Looks through the list of ServerFunctions and looks at each function's key value (which
 * would be the value of SurverFunction.getName() for each function). When a function has the same
 * key name as the value of the string parameter 'name', then the returned value parameter is set
 * the value returned by ServerFunction.get_proj_func(). If the ServerFunction _is not_  a projection
 * function then the return value will be 0 (null) and the search for matching function will continue.
 * If the ServerFunction _is_ a projection then the returned value will be non-zero and the search will
 * return true (it found the thing) and the returned value parameter *f will have it's value set
 * to the projection function.
 *
 *  @brief Find a projection function with a given name in the function list.
 *  @param name A string containing the name of the function to find.
 *  @param *f   A returned value parameter through which a point to the desired function is returned.
 *
 */
bool ServerFunctionsList::find_function(const string &name, proj_func *f) const
{

#if 0
    if (d_proj_func_list.empty())
        return false;

    map<string, proj_func>::const_iterator i = d_proj_func_list.begin();
    while(i != d_proj_func_list.end()) {
        if (name == (*i).first && (*f = (*i).second)) {
            return true;
        }
        ++i;
    }

    return false;
#endif

    if (func_list.empty())
        return false;

    std::pair <std::multimap<string,libdap::ServerFunction *>::const_iterator, std::multimap<string,libdap::ServerFunction *>::const_iterator> ret;
    ret = func_list.equal_range(name);
    for (std::multimap<string,libdap::ServerFunction *>::const_iterator it=ret.first; it!=ret.second; ++it) {
        if (name == it->first && (*f = it->second->get_proj_func())){
            return true;
        }
    }
    return false;

}



/** @brief Returns an iterator pointing to the first key pair in the ServerFunctionList. */
std::multimap<string,libdap::ServerFunction *>::iterator ServerFunctionsList::begin()
{
    return func_list.begin();
}

/** @brief Returns an iterator pointing to the last key pair in the ServerFunctionList. */
std::multimap<string,libdap::ServerFunction *>::iterator ServerFunctionsList::end()
{
    return func_list.end();
}


/**
 *
 *
 * @brief Returns the ServerFunction pointed to by the passed iterator.
 *
 */
libdap::ServerFunction *ServerFunctionsList::getFunction(std::multimap<string,libdap::ServerFunction *>::iterator it)
{
    return (*it).second;
}






#if 0
/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance along with information about
 * this catalog directory.
 *
 * @param strm C++ i/o stream to dump the information to
 */
void ServerFunctionsList::dump(ostream &strm) const
{
    strm << BESIndent::LMarg << "ServerFunctionsList::dump - (" << (void *) this << ")" << endl;
    BESIndent::Indent();

    if (d_btp_func_list.size() > 0) {
        strm << BESIndent::LMarg << "registered btp functions:" << endl;
        BESIndent::Indent();
        map<string, btp_func>::const_iterator i = d_btp_func_list.begin();
        map<string, btp_func>::const_iterator e = d_btp_func_list.end();
        while (i != e) {
            strm << (*i).first << endl;
            ++i;
        }
        BESIndent::UnIndent();
    }
    else {
        strm << BESIndent::LMarg << "registered btp functions: none" << endl;
    }

    if (d_bool_func_list.size() > 0) {
        strm << BESIndent::LMarg << "registered bool functions:" << endl;
        BESIndent::Indent();
        map<string, bool_func>::const_iterator i = d_bool_func_list.begin();
        map<string, bool_func>::const_iterator e = d_bool_func_list.end();
        while (i != e) {
            strm << (*i).first << endl;
            ++i;
        }
        BESIndent::UnIndent();
    }
    else {
        strm << BESIndent::LMarg << "registered bool functions: none" << endl;
    }

    if (d_proj_func_list.size() > 0) {
        strm << BESIndent::LMarg << "registered projection functions:" << endl;
        BESIndent::Indent();
        map<string, proj_func>::const_iterator i = d_proj_func_list.begin();
        map<string, proj_func>::const_iterator e = d_proj_func_list.end();
        while (i != e) {
            strm << (*i).first << endl;
            ++i;
        }
        BESIndent::UnIndent();
    }
    else {
        strm << BESIndent::LMarg << "registered projection functions: none" << endl;
    }

    BESIndent::UnIndent();
}
#endif

ServerFunctionsList *
ServerFunctionsList::TheList()
{
    if (d_instance == 0) {
        d_instance = new ServerFunctionsList;
    }
    return d_instance;
}


void ServerFunctionsList::getFunctionNames(vector<string> *names){
    std::multimap<string,libdap::ServerFunction *>::iterator fit;
    for(fit=func_list.begin(); fit!=func_list.end() ; fit++){
        ServerFunction *func = fit->second;
        names->push_back(func->getName());
    }
}


}
