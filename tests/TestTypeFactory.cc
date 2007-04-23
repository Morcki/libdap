
// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2005 OPeNDAP, Inc.
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


#include <string>

#include "TestByte.h"
#include "TestInt16.h"
#include "TestUInt16.h"
#include "TestInt32.h"
#include "TestUInt32.h"
#include "TestFloat32.h"
#include "TestFloat64.h"
#include "TestStr.h"
#include "TestUrl.h"
#include "TestArray.h"
#include "TestStructure.h"
#include "TestSequence.h"
#include "TestGrid.h"

#include "BaseTypeFactory.h"
#include "TestTypeFactory.h"

#include "debug.h"

Byte *
TestTypeFactory::NewByte(const string &n, const string &ds ) const 
{ 
    return new TestByte(n, ds);
}

Int16 *
TestTypeFactory::NewInt16(const string &n, const string &ds ) const 
{ 
    return new TestInt16(n, ds); 
}

UInt16 *
TestTypeFactory::NewUInt16(const string &n, const string &ds ) const 
{ 
    return new TestUInt16(n, ds);
}

Int32 *
TestTypeFactory::NewInt32(const string &n, const string &ds ) const 
{ 
    DBG(cerr << "Inside TestTypeFactory::NewInt32" << endl);
    return new TestInt32(n, ds);
}

UInt32 *
TestTypeFactory::NewUInt32(const string &n, const string &ds ) const 
{ 
    return new TestUInt32(n, ds);
}

Float32 *
TestTypeFactory::NewFloat32(const string &n, const string &ds ) const 
{ 
    return new TestFloat32(n, ds);
}

Float64 *
TestTypeFactory::NewFloat64(const string &n, const string &ds ) const 
{ 
    return new TestFloat64(n, ds);
}

Str *
TestTypeFactory::NewStr(const string &n, const string &ds ) const 
{ 
    return new TestStr(n, ds);
}

Url *
TestTypeFactory::NewUrl(const string &n, const string &ds ) const 
{ 
    return new TestUrl(n, ds);
}

Array *
TestTypeFactory::NewArray(const string &n , BaseType *v, const string &ds) const
{ 
    return new TestArray(n, v, ds);
}

Structure *
TestTypeFactory::NewStructure(const string &n, const string &ds ) const 
{ 
    return new TestStructure(n, ds);
}

Sequence *
TestTypeFactory::NewSequence(const string &n, const string &ds ) const 
{
    DBG(cerr << "Inside TestTypeFactory::NewSequence" << endl);
    return new TestSequence(n, ds);
}

Grid *
TestTypeFactory::NewGrid(const string &n, const string &ds ) const 
{ 
    return new TestGrid(n, ds);
}

