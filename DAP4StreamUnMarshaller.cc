// DAP4StreamUnMarshaller.cc

// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2012 OPeNDAP, Inc.
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

#include "config.h"
#include "DAP4StreamUnMarshaller.h"

//#include <cstring> // for memcpy
#include <iostream>
#include <iomanip>

#include <string>
#include <sstream>

//#define DODS_DEBUG2 1
//#define DODS_DEBUG 1

//#include "Str.h"
//#include "Vector.h"
//#include "Array.h"

#include "util.h"
#include "XDRUtils.h"
#include "InternalErr.h"
#include "debug.h"

namespace libdap {

DAP4StreamUnMarshaller::DAP4StreamUnMarshaller(istream &in, bool isBigEndian)
    : d_in( in ), d_isBigEndian(isBigEndian), d_buf(0)
{
    if (!d_buf)
        d_buf = (char *) malloc(sizeof(dods_float64));
    if (!d_buf)
        throw Error("Failed to allocate memory for data serialization.");

    xdrmem_create(&d_source, d_buf, sizeof(dods_float64), XDR_DECODE);

    // This will cause exceptions to be thrown on i/o errors. The exception
    // will be ostream::failure
    d_in.exceptions(istream::failbit | istream::badbit);

}

DAP4StreamUnMarshaller::~DAP4StreamUnMarshaller( )
{
}

DAP4StreamUnMarshaller::checksum DAP4StreamUnMarshaller::get_checksum()
{
    checksum c;
    d_in.read(reinterpret_cast<char*>(&c), c_md5_length);

    return c;
}

string DAP4StreamUnMarshaller::get_checksum(DAP4StreamUnMarshaller::checksum c)
{
    unsigned char *md = reinterpret_cast<unsigned char*>(&c);

    ostringstream oss;
    oss.setf(ios::hex, ios::basefield);
    for (unsigned int i = 0; i < c_md5_length; ++i) {
        oss << setfill('0') << setw(2) << (unsigned int)md[i];
    }

    return oss.str();
}

void
DAP4StreamUnMarshaller::get_byte( dods_byte &val )
{
    d_in.read(reinterpret_cast<char*>(&val), sizeof(dods_byte));
}

void
DAP4StreamUnMarshaller::get_int16( dods_int16 &val )
{
    d_in.read(reinterpret_cast<char*>(&val), sizeof(dods_int16));
}

void
DAP4StreamUnMarshaller::get_int32( dods_int32 &val )
{
    d_in.read(reinterpret_cast<char*>(&val), sizeof(dods_int32));
}

void
DAP4StreamUnMarshaller::get_int64( dods_int64 &val )
{
    d_in.read(reinterpret_cast<char*>(&val), sizeof(dods_int64));
}

void
DAP4StreamUnMarshaller::get_float32( dods_float32 &val )
{
    if (std::numeric_limits<float>::is_iec559) {
        d_in.read(reinterpret_cast<char*>(&val), sizeof(dods_float32));
    }
    else {
        xdr_setpos( &d_source, 0);
        d_in.read(d_buf, sizeof(dods_float32));

        if (!xdr_float(&d_source, &val))
            throw Error("Network I/O Error. Could not read float 64 data.");
    }
}

void
DAP4StreamUnMarshaller::get_float64( dods_float64 &val )
{
    if (std::numeric_limits<float>::is_iec559) {
        d_in.read(reinterpret_cast<char*>(&val), sizeof(dods_float64));
    }
    else {
        xdr_setpos( &d_source, 0);
        d_in.read(d_buf, sizeof(dods_float64));

        if (!xdr_double(&d_source, &val))
            throw Error("Network I/O Error. Could not read float 64 data.");
    }
}

void
DAP4StreamUnMarshaller::get_uint16( dods_uint16 &val )
{
    d_in.read(reinterpret_cast<char*>(&val), sizeof(dods_uint16));
}

void
DAP4StreamUnMarshaller::get_uint32( dods_uint32 &val )
{
    d_in.read(reinterpret_cast<char*>(&val), sizeof(dods_uint32));
}

void
DAP4StreamUnMarshaller::get_uint64( dods_uint64 &val )
{
    d_in.read(reinterpret_cast<char*>(&val), sizeof(dods_uint64));
}

/**
 * Read a varint (128-bit varying integer). Not the most optimized version
 * possible. It would be better if the values were in memory and we could
 * operate on them without a separate read for each byte.
 */
dods_uint64 DAP4StreamUnMarshaller::get_length_prefix()
{
    uint8_t b;
    int count = 0;
    dods_uint64 result = 0;
    do {
        d_in.read(reinterpret_cast<char*>(&b), 1);

        uint64_t v = (b & 0x7f) << 7 * count;
        result += v; // is v needed? Does it matter?
        count++;

    } while (b & 0x80);

    return result;
}


void
DAP4StreamUnMarshaller::get_str( string &val )
{
    dods_int64 len = get_length_prefix();
    vector<char> raw(len+1);
    d_in.read(&raw[0], len);

    val.reserve(len);
    val.assign(&raw[0], len);
}

void
DAP4StreamUnMarshaller::get_url( string &val )
{
    get_str( val ) ;
}

/**
 * Get opaque data when the size of the data to read is known.
 *
 * @param val Pointer to 'len' bytes; store the data here.
 * @param len Number of bytes referenced adn expected
 * @exception Error if the number of bytes indicated in the stream does not
 * match 'len'
 */
void
DAP4StreamUnMarshaller::get_opaque( char *val, unsigned int len )
{
    dods_int64 rlen = get_length_prefix();
    if (len != rlen)
        throw Error("Expected opaque data of " + long_to_string(len)
                + " bytes, but got " + long_to_string(rlen) + " instead.");

    d_in.read(val, len);
}

/**
 * Get opaque data when the size of the data to be read is not known in
 * advance.
 *
 * @param val Value-result parameter for the data; caller must delete.
 * @param len value-result parameter for the length of the data
 */
void
DAP4StreamUnMarshaller::get_opaque( char **val, unsigned int &len )
{
    len = get_length_prefix();

    *val = new char[len];
    d_in.read(*val, len);
}

void
DAP4StreamUnMarshaller::get_vector( char *val, unsigned int num )
{
    d_in.read(val, num);
}

/**
 * @todo recode this so that it does not copy data to a new buffer but
 * serializes directly to the stream (element by element) and compare the
 * run times.
 */
template <class T>
void DAP4StreamUnMarshaller::m_deserialize_reals(char *val, unsigned int num, Type type)
{
    dods_uint64 size = num * sizeof(T);
    char *buf = (char*)malloc(size);
    XDR xdr;
    xdrmem_create(&xdr, buf, size, XDR_DECODE);
    try {
        xdr_setpos(&d_source, 0);
        d_in.read(buf, size);

        if(!xdr_array(&xdr, &val, (unsigned int *)&num, size, sizeof(T), XDRUtils::xdr_coder(type)))
            throw InternalErr(__FILE__, __LINE__, "Error deserializing a Float64 array");

        if (xdr_getpos(&xdr) != size)
            throw InternalErr(__FILE__, __LINE__, "Error deserializing a Float64 array");
    }
    catch (...) {
        xdr_destroy(&xdr);
        throw;
    }
    xdr_destroy(&xdr);
}

void
DAP4StreamUnMarshaller::get_vector( char *val, unsigned int num, int width, Type type )
{
    if (type == dods_float32_c && !std::numeric_limits<float>::is_iec559) {
        // If not using IEEE 754, use XDR to get it that way.
        m_deserialize_reals<dods_float32>(val, num, type);
    }
    else if (type == dods_float64_c && !std::numeric_limits<double>::is_iec559) {
        m_deserialize_reals<dods_float64>(val, num, type);
    }
    else {
        d_in.read(val, num * width);
    }
}

void
DAP4StreamUnMarshaller::get_varying_vector( char **val, unsigned int &num )
{
    get_opaque(val, num);
}

void
DAP4StreamUnMarshaller::get_varying_vector( char **val, unsigned int &num, int width, Type type )
{
    num = get_length_prefix();

    int size = num * width;
    *val = new char[size];

    if (type == dods_float32_c && !std::numeric_limits<float>::is_iec559) {
        // If not using IEEE 754, use XDR to get it that way.
        m_deserialize_reals<dods_float32>(*val, num, type);
    }
    else if (type == dods_float64_c && !std::numeric_limits<double>::is_iec559) {
        m_deserialize_reals<dods_float64>(*val, num, type);
    }
    else {
        d_in.read(*val, size);
    }
}

void
DAP4StreamUnMarshaller::dump(ostream &strm) const
{
    strm << DapIndent::LMarg << "DAP4StreamUnMarshaller::dump - ("
         << (void *)this << ")" << endl ;
}

} // namespace libdap

