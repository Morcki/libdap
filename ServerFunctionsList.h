// ServerFunctionsList.h

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

#ifndef I_ServerFunctionsList_h
#define I_ServerFunctionsList_h 1

#include <map>
#include <string>

#include "ServerFunction.h"

#include <expr.h>
#if 0
using std::map;
using std::string;
#endif

namespace libdap {

class ConstraintEvaluator;

//#include "BESObj.h"

class ServerFunctionsList {
private:
    static ServerFunctionsList * d_instance;

#if 0
    std::map<std::string, libdap::btp_func> d_btp_func_list;
    std::map<std::string, libdap::bool_func> d_bool_func_list;
    std::map<std::string, libdap::proj_func> d_proj_func_list;
#endif

    std::multimap<std::string, libdap::ServerFunction *> func_list;



protected:
    ServerFunctionsList(void);

public:
    virtual ~ServerFunctionsList(void);

#if 0
    virtual bool add_function(std::string name, libdap::btp_func func);
    virtual bool add_function(std::string name, libdap::bool_func func);
    virtual bool add_function(std::string name, libdap::proj_func func);
#endif


#if 0
    virtual void store_functions(libdap::ConstraintEvaluator &ce);
#endif


    virtual void add_function(libdap::ServerFunction *func);

    virtual bool find_function(const std::string &name, libdap::bool_func *f) const;
    virtual bool find_function(const std::string &name, libdap::btp_func  *f) const;
    virtual bool find_function(const std::string &name, libdap::proj_func *f) const;

    //virtual void dump(ostream &strm) const;

    std::multimap<string,libdap::ServerFunction *>::iterator begin();
    std::multimap<string,libdap::ServerFunction *>::iterator end();
    ServerFunction *getFunction(std::multimap<string,libdap::ServerFunction *>::iterator it);

    virtual void getFunctionNames(vector<string> *names);

    static ServerFunctionsList * TheList();
};

}

#endif // I_ServerFunctionsList_h
