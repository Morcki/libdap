// XDRStreamMarshaller.cc

// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2002,2003 OPeNDAP, Inc.
// Author: Patrick West <pwest@ucar.edu>
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

// (c) COPYRIGHT URI/MIT 1994-1999
// Please read the full copyright statement in the file COPYRIGHT_URI.
//
// Authors:
//      pwest       Patrick West <pwest@ucar.edu>

#include "XDRStreamMarshaller.h"

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

//#define DODS_DEBUG 1

#include "Vector.h"
#include "util.h"
#include "debug.h"

namespace libdap {

char *XDRStreamMarshaller::_buf = 0;

#define XDR_DAP_BUFF_SIZE 256

/** Build an instance of XDRStreamMarshaller. Bind the C++ stream out to this
 * instance. If the checksum parameter is true, initialize a checksum buffer
 * and enable the use of the reset_checksum() and get_checksum() methods.
 *
 * @param out Write to this stream object.
 * @param checksum If true, compute checksums. False by default
 * @param write_data If true, write data values. True by default
 */
XDRStreamMarshaller::XDRStreamMarshaller(ostream &out, bool checksum, bool write_data) :
    _sink(0), _out(out), _md5(0), _write_data(write_data), _checksum_ctx_valid(false)
{
    if (!_buf)
        _buf = (char *) malloc(XDR_DAP_BUFF_SIZE);
    if (!_buf)
        throw Error("Failed to allocate memory for data serialization.");

    _sink = new XDR;
    xdrmem_create(_sink, _buf, XDR_DAP_BUFF_SIZE, XDR_ENCODE);

    if (checksum) {
        _md5 = reinterpret_cast<MD5_CTX*>(new char[sizeof(MD5_CTX)]);
    }
}

XDRStreamMarshaller::XDRStreamMarshaller() :
    Marshaller(), _sink(0), _out(cout)
{
    throw InternalErr(__FILE__, __LINE__, "Default constructor not implemented.");
}

XDRStreamMarshaller::XDRStreamMarshaller(const XDRStreamMarshaller &m) :
    Marshaller(m), _sink(0), _out(cout)
{
    throw InternalErr(__FILE__, __LINE__, "Copy constructor not implemented.");
}

XDRStreamMarshaller &
XDRStreamMarshaller::operator=(const XDRStreamMarshaller &)
{
    throw InternalErr(__FILE__, __LINE__, "Copy operator not implemented.");

    return *this;
}

XDRStreamMarshaller::~XDRStreamMarshaller()
{
    if (_sink)
        xdr_destroy(_sink); //delete_xdrstdio(_sink);

    _sink = 0;
}

/** Initialize the checksum buffer. This resets the checksum calculation.
 * @exception InternalErr if called when the object was created without
 * checksum support.
 */
void XDRStreamMarshaller::reset_checksum()
{
    if (_md5 == 0)
        throw InternalErr( __FILE__, __LINE__, "checksum_init() called by checksum is not enabled.");

    if (MD5_Init(_md5) == 0)
        throw Error("Error initializing the checksum buffer.");

    _checksum_ctx_valid = true;
}

/** Get the current checksum. It is not possible to continue computing the
 * checksum once this has been called.
 * @exception InternalErr if called when the object was created without
 * checksum support or if called when the checksum has already been returned.
 */
string XDRStreamMarshaller::get_checksum()
{
    if (_md5 == 0)
        throw InternalErr( __FILE__, __LINE__, "checksum_init() called by checksum is not enabled.");

    if (!_checksum_ctx_valid)
        throw InternalErr( __FILE__, __LINE__, "Invalid checksum context.");

    // Setting this here ensures that we call get_checksum() only once for
    // a context. The 'Final()' 'erases' the context so the next checksum is
    // bogus.
    _checksum_ctx_valid = false;

    vector<unsigned char> md(MD5_DIGEST_LENGTH);
    if (MD5_Final(&md[0], _md5) == 0)
        throw Error("Error computing the checksum.");

    ostringstream oss;
    oss.setf ( ios::hex, ios::basefield );
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        oss << setfill('0') << setw(2) << (unsigned int)md[i];
    }

    return oss.str();
}

void XDRStreamMarshaller::checksum_update(const void *data, unsigned long len)
{
    if (_md5 == 0)
        throw InternalErr( __FILE__, __LINE__, "checksum_init() called by checksum is not enabled.");

    if (!_checksum_ctx_valid)
        throw InternalErr( __FILE__, __LINE__, "Invalid checksum context.");

    if (MD5_Update(_md5, data, len) == 0) {
        _checksum_ctx_valid = false;
        throw Error("Error computing the checksum.");
    }
}

void XDRStreamMarshaller::put_byte(dods_byte val)
{
    if (_md5)
        checksum_update(&val, sizeof(dods_byte));

    if (_write_data) {
    DBG( std::cerr << "put_byte: " << val << std::endl );

    if (!xdr_setpos( _sink, 0 ))
        throw Error("Network I/O Error. Could not send byte data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    if (!xdr_char(_sink, (char *) &val))
        throw Error("Network I/O Error. Could not send byte data.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    unsigned int bytes_written = xdr_getpos( _sink );
    if (!bytes_written)
        throw Error("Network I/O Error. Could not send byte data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    _out.write(_buf, bytes_written);
    }
}

void XDRStreamMarshaller::put_int16(dods_int16 val)
{
    if (_md5)
        checksum_update(&val, sizeof(dods_int16));

    if (_write_data) {
    if (!xdr_setpos( _sink, 0 ))
        throw Error("Network I/O Error. Could not send int 16 data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    if (!XDR_INT16(_sink, &val))
        throw Error("Network I/O Error. Could not send int 16 data.\nThis may be due to a bug in libdap, on the server or a\nproblem with the network connection.");

    unsigned int bytes_written = xdr_getpos( _sink );
    if (!bytes_written)
        throw Error("Network I/O Error. Could not send int 16 data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    _out.write(_buf, bytes_written);
    }
}

void XDRStreamMarshaller::put_int32(dods_int32 val)
{
    if (_md5)
        checksum_update(&val, sizeof(dods_int32));

    if (_write_data) {
    if (!xdr_setpos( _sink, 0 ))
        throw Error("Network I/O Error. Could not send int 32 data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    if (!XDR_INT32(_sink, &val))
        throw Error("Network I/O Error. Culd not read int 32 data.\nThis may be due to a bug in libdap, on the server or a\nproblem with the network connection.");

    unsigned int bytes_written = xdr_getpos( _sink );
    if (!bytes_written)
        throw Error("Network I/O Error. Could not send int 32 data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    _out.write(_buf, bytes_written);
    }
}

void XDRStreamMarshaller::put_float32(dods_float32 val)
{
    if (_md5)
        checksum_update(&val, sizeof(dods_float32));

    if (_write_data) {
    if (!xdr_setpos( _sink, 0 ))
        throw Error("Network I/O Error. Could not send float 32 data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    if (!xdr_float(_sink, &val))
        throw Error("Network I/O Error. Could not send float 32 data.\nThis may be due to a bug in libdap, on the server or a\nproblem with the network connection.");

    unsigned int bytes_written = xdr_getpos( _sink );
    if (!bytes_written)
        throw Error("Network I/O Error. Could not send float 32 data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    _out.write(_buf, bytes_written);
    }
}

void XDRStreamMarshaller::put_float64(dods_float64 val)
{
    if (_md5)
        checksum_update(&val, sizeof(dods_float64));

    if (_write_data) {
    if (!xdr_setpos( _sink, 0 ))
        throw Error("Network I/O Error. Could not send float 64 data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    if (!xdr_double(_sink, &val))
        throw Error("Network I/O Error. Could not send float 64 data.\nThis may be due to a bug in libdap, on the server or a\nproblem with the network connection.");

    unsigned int bytes_written = xdr_getpos( _sink );
    if (!bytes_written)
        throw Error("Network I/O Error. Could not send float 64 data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    _out.write(_buf, bytes_written);
    }
}

void XDRStreamMarshaller::put_uint16(dods_uint16 val)
{
    if (_md5)
        checksum_update(&val, sizeof(dods_uint16));

    if (_write_data) {
    if (!xdr_setpos( _sink, 0 ))
        throw Error("Network I/O Error. Could not send uint 16 data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    if (!XDR_UINT16(_sink, &val))
        throw Error("Network I/O Error. Could not send uint 16 data. This may be due to a\nbug in libdap or a problem with the network connection.");

    unsigned int bytes_written = xdr_getpos( _sink );
    if (!bytes_written)
        throw Error("Network I/O Error. Could not send uint 16 data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    _out.write(_buf, bytes_written);
    }
}

void XDRStreamMarshaller::put_uint32(dods_uint32 val)
{
    if (_md5)
        checksum_update(&val, sizeof(dods_uint32));

    if (_write_data) {
    if (!xdr_setpos( _sink, 0 ))
        throw Error("Network I/O Error. Could not send uint 32 data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    if (!XDR_UINT32(_sink, &val))
        throw Error("Network I/O Error. Could not send uint 32 data. This may be due to a\nbug in libdap or a problem with the network connection.");

    unsigned int bytes_written = xdr_getpos( _sink );
    if (!bytes_written)
        throw Error("Network I/O Error. Could not send uint 32 data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    _out.write(_buf, bytes_written);
    }
}

void XDRStreamMarshaller::put_str(const string &val)
{
    if (_md5)
        checksum_update(val.c_str(), val.length());

    if (_write_data) {
    int size = val.length() + 8;
    // Replace this malloc with a vector<char>.
    // ...and maybe elsewhere in this class... jhrg 3/9/12
    char *str_buf = (char *) malloc(size);

    if (!str_buf) {
        throw Error("Failed to allocate memory for string data serialization.");
    }

    XDR *str_sink = new XDR;
    xdrmem_create(str_sink, str_buf, size, XDR_ENCODE);

    if (!xdr_setpos( str_sink, 0 )) {
        delete_xdrstdio(str_sink);
        free(str_buf);
        throw Error("Network I/O Error. Could not send string data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");
    }

    const char *out_tmp = val.c_str();
    if (!xdr_string(str_sink, (char **) &out_tmp, size)) {
        delete_xdrstdio(str_sink);
        free(str_buf);
        throw Error("Network I/O Error. Could not send string data.\nThis may be due to a bug in libdap, on the server or a\nproblem with the network connection.");
    }

    unsigned int bytes_written = xdr_getpos( str_sink );
    if (!bytes_written) {
        delete_xdrstdio(str_sink);
        free(str_buf);
        throw Error("Network I/O Error. Could not send string data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");
    }

    _out.write(str_buf, bytes_written);

    delete_xdrstdio(str_sink);
    free(str_buf);
    }
}

void XDRStreamMarshaller::put_url(const string &val)
{
    put_str(val);
}

void XDRStreamMarshaller::put_opaque(char *val, unsigned int len)
{
    if (_md5)
        checksum_update(&val, len);

    if (_write_data) {
    if (len > XDR_DAP_BUFF_SIZE)
        throw Error("Network I/O Error. Could not send opaque data - length of opaque data larger than allowed");

    if (!xdr_setpos( _sink, 0 ))
        throw Error("Network I/O Error. Could not send opaque data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    if (!xdr_opaque(_sink, val, len))
        throw Error("Network I/O Error. Could not send opaque data.\nThis may be due to a bug in libdap, on the server or a\nproblem with the network connection.");

    unsigned int bytes_written = xdr_getpos( _sink );
    if (!bytes_written)
        throw Error("Network I/O Error. Could not send opaque data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    _out.write(_buf, bytes_written);
    }
}

void XDRStreamMarshaller::put_int(int val)
{
#if 0
    if (_md5)
        checksum_update(&val, sizeof(int));
#endif
    if (_write_data) {
    if (!xdr_setpos( _sink, 0 ))
        throw Error("Network I/O Error. Could not send int data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    if (!xdr_int(_sink, &val))
        throw Error("Network I/O Error(1). Could not send int data.\nThis may be due to a bug in libdap or a\nproblem with the network connection.");

    unsigned int bytes_written = xdr_getpos( _sink );
    if (!bytes_written)
        throw Error("Network I/O Error. Could not send int data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");

    _out.write(_buf, bytes_written);
    }
}

void XDRStreamMarshaller::put_vector(char *val, int num, Vector &)
{
    if (!val)
        throw InternalErr(__FILE__, __LINE__, "Could not send byte vector data. Buffer pointer is not set.");

    if (_md5)
        checksum_update(val, num);

    if (_write_data) {
    // write the number of members of the array being written and then set the position to 0
    put_int(num);

    // this is the word boundary for writing xdr bytes in a vector.
    unsigned int add_to = 8;

    char *byte_buf = (char *) malloc(num + add_to);
    if (!byte_buf) {
        throw Error("Failed to allocate memory for byte vector data serialization.");
    }

    XDR *byte_sink = new XDR;
    xdrmem_create(byte_sink, byte_buf, num + add_to, XDR_ENCODE);

    if (!xdr_setpos( byte_sink, 0 )) {
        delete_xdrstdio(byte_sink);
        free(byte_buf);
        throw Error("Network I/O Error. Could not send byte vector data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");
    }

    if (!xdr_bytes(byte_sink, (char **) &val, (unsigned int *) &num, num + add_to)) {
        delete_xdrstdio(byte_sink);
        free(byte_buf);
        throw Error("Network I/O Error(2). Could not send byte vector data.\nThis may be due to a bug in libdap or a\nproblem with the network connection.");
    }

    unsigned int bytes_written = xdr_getpos( byte_sink );
    if (!bytes_written) {
        delete_xdrstdio(byte_sink);
        free(byte_buf);
        throw Error("Network I/O Error. Could not send byte vector data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");
    }

    _out.write(byte_buf, bytes_written);

    delete_xdrstdio(byte_sink);
    free(byte_buf);
    }
}

void XDRStreamMarshaller::put_vector(char *val, int num, int width, Vector &vec)
{
    if (!val)
        throw InternalErr(__FILE__, __LINE__, "Buffer pointer is not set.");

    if (_md5)
        checksum_update(val, num * width);

    if (_write_data) {
    // write the number of array members being written, then set the position back to 0
    put_int(num);

    int use_width = width;
    if (use_width < 4)
        use_width = 4;

    // the size is the number of elements num times the width of each
    // element, then add 4 bytes for the number of elements
    int size = (num * use_width) + 4;

    // allocate enough memory for the elements
    char *vec_buf = (char *) malloc(size);
    if (!vec_buf) {
        free(vec_buf);
        throw Error("Failed to allocate memory for vector data serialization.");
    }

    XDR *vec_sink = new XDR;
    xdrmem_create(vec_sink, vec_buf, size, XDR_ENCODE);

    // set the position of the sink to 0, we're starting at the beginning
    if (!xdr_setpos( vec_sink, 0 )) {
        delete_xdrstdio(vec_sink);
        free(vec_buf);
        throw Error("Network I/O Error. Could not send vector data - unable to set stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");
    }

    BaseType *var = vec.var();

    // write the array to the buffer
    if (!xdr_array(vec_sink, (char **) &val, (unsigned int *) &num, size, width, XDRUtils::xdr_coder(var->type()))) {
        delete_xdrstdio(vec_sink);
        free(vec_buf);
        throw Error("Network I/O Error(2). Could not send vector data.\nThis may be due to a bug in libdap or a\nproblem with the network connection.");
    }

    // how much was written to the buffer
    unsigned int bytes_written = xdr_getpos( vec_sink );
    if (!bytes_written) {
        delete_xdrstdio(vec_sink);
        free(vec_buf);
        throw Error("Network I/O Error. Could not send vector data - unable to get stream position.\nThis may be due to a bug in DODS, on the server or a\nproblem with the network connection.");
    }

    // write that much out to the output stream
    _out.write(vec_buf, bytes_written);

    delete_xdrstdio(vec_sink);
    free(vec_buf);
    }
}

void XDRStreamMarshaller::dump(ostream &strm) const
{
    strm << DapIndent::LMarg << "XDRStreamMarshaller::dump - (" << (void *) this << ")" << endl;
}

} // namespace libdap

