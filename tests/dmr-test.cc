// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2002,2003,2013 OPeNDAP, Inc.
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

// Test the DMR parser

#include "config.h"

#include <stdint.h>
#include <cstring>

#include <fstream>

#include "crc.h"

#include <GetOpt.h>

#include "DMR.h"
#include "D4StreamUnMarshaller.h"
#include "chunked_ostream.h"
#include "chunked_istream.h"

#include "util.h"
#include "InternalErr.h"
#include "Error.h"

#include "D4ResponseBuilder.h"
#include "ConstraintEvaluator.h"

#include "D4ParserSax2.h"
#include "D4TestTypeFactory.h"
#include "TestCommon.h"

#include "util.h"
#include "mime_util.h"
#include "debug.h"

int test_variable_sleep_interval = 0;   // Used in Test* classes for testing timeouts.

using namespace libdap;

/**
 * Open the named XML file and parse it, assuming that it contains a DMR.
 * @param name The name of the DMR XML file (or '-' for stdin)
 * @param debug True if the debug mode of the parse should be used
 * @param print Once parsed, should the DMR object be printed?
 * @return true if the parse worked, false otherwise
 */
DMR *
test_dap4_parser(const string &name, bool debug, bool print)
{
    D4TestTypeFactory *factory = new D4TestTypeFactory;
    DMR *dataset = new DMR(factory, path_to_filename(name));

    try {
        D4ParserSax2 parser;
        if (name == "-") {
            parser.intern(cin, dataset, debug);
        }
        else {
            fstream in(name.c_str(), ios_base::in);
            parser.intern(in, dataset, debug);
        }
    }
    catch(...) {
        delete factory;
        delete dataset;
        throw;
    }

    cout << "Parse successful" << endl;

    if (print) {
        XMLWriter xml("    ");
        dataset->print_dap4(xml, false);
        cout << xml.get_doc() << endl;
    }

    return dataset;
}

/**
 * Should the changing values - meant to mimic the DTS - be used?
 * @param dmr Set for this DMR
 * @param state True to use the DTS-like values, false otherwise
 */
void
set_series_values(DMR *dmr, bool state)
{
	if (state == true)
		dmr->root()->set_read_p(false);

	TestCommon *tc = dynamic_cast<TestCommon*>(dmr->root());
	if (tc)
		tc->set_series_values(state);
	else
		cerr << "Could not cast root group to TestCommon (" << dmr->root()->type_name() << ", " << dmr->root()->name() << ")" << endl;
}

/**
 * Call the parser and then serialize the resulting DMR after applying the
 * constraint. The persistent representation is written to a file. The file
 * is name '<name>_data.bin'.
 *
 * @param dataset
 * @param constraint
 * @param series_values
 * @return The name of the file that hods the response.
 */
string
send_data(DMR *dataset, const string &constraint, bool series_values, bool ce_parse_debug)
{
    set_series_values(dataset, series_values);

    // ConstraintEvaluator eval;	// This is a place holder. jhrg 9/6/13
    D4ResponseBuilder rb;
    rb.set_ce(constraint);
    rb.set_dataset_name(dataset->name());
#if 0
    // TODO Remove once real CE evaluator is written. jhrg 9/6/13
    // Mark all variables to be sent in their entirety.
    dataset->root()->set_send_p(true);
#endif
    string file_name = dataset->name() + "_data.bin";
    ofstream out(file_name.c_str(), ios::out|ios::trunc|ios::binary);

    rb.send_data_dmr(out, *dataset, /*eval,*/ true, ce_parse_debug);
    out.close();

    return file_name;
}

void
intern_data(DMR *dataset, const string &/*constraint*/, bool series_values)
{
    set_series_values(dataset, series_values);

    ConstraintEvaluator eval;	// This is a place holder. jhrg 9/6/13

    // TODO Remove once a real CE evaluator is written. jhrg 9/6/13
    // Mark all variables to be sent in their entirety.
    dataset->root()->set_send_p(true);

    Crc32 checksum;
    dataset->root()->intern_data(checksum, *dataset, eval);
}

DMR *
read_data_plain(const string &file_name, bool debug)
{
    D4BaseTypeFactory *factory = new D4BaseTypeFactory;
    DMR *dmr = new DMR(factory, "Test_data");

    fstream in(file_name.c_str(), ios::in|ios::binary);

    // Gobble up the response's initial set of MIME headers. Normally
    // a client would extract information from these headers.
    remove_mime_header(in);

    chunked_istream cis(in, CHUNK_SIZE);

    // parse the DMR, stopping when the boundary is found.
    try {
        // force chunk read
        // get chunk size
        int chunk_size = cis.read_next_chunk();
        // get chunk
        char chunk[chunk_size];
        cis.read(chunk, chunk_size);
        // parse char * with given size
    	D4ParserSax2 parser;
    	// '-2' to discard the CRLF pair
        parser.intern(chunk, chunk_size-2, dmr, debug);
    }
    catch(Error &e) {
    	delete factory;
    	delete dmr;
    	cerr << "Exception: " << e.get_error_message() << endl;
    	return 0;
    }
    catch(std::exception &e) {
    	delete factory;
    	delete dmr;
    	cerr << "Exception: " << e.what() << endl;
    	return 0;
    }
    catch(...) {
    	delete factory;
    	delete dmr;
    	cerr << "Exception: unknown error" << endl;
    	return 0;
    }

    D4StreamUnMarshaller um(cis, cis.twiddle_bytes());

    dmr->root()->deserialize(um, *dmr);

    return dmr;
}

static void usage()
{
    cerr << "Usage: dmr-test -p|s|t|i <file> [-c <expr>] [-d -x -e]" << endl
            << "p: Parse a file (use "-" for stdin)" << endl
            << "s: Send - parse and then 'send' a response to a file" << endl
            << "t: Transmit - parse, send and then read the response file" << endl
            << "i: Intern values" << endl
            << "c: Constraint expression " << endl
            << "d: turn on detailed xml parser debugging" << endl
            << "D: turn on detailed ce parser debugging" << endl
            << "x: print the binary object(s) built by the parse, send, trans or intern operations." << endl
            << "e: use sEries values." << endl;
}

int
main(int argc, char *argv[])
{
    GetOpt getopt(argc, argv, "p:s:t:i:c:xdDeh?");
    int option_char;
    bool parse = false;
    bool debug = false;
    bool print = false;
    bool send = false;
    bool trans = false;
    bool intern = false;
    bool series_values = false;
    bool constrained = false;
    bool ce_parser_debug = false;
    string name = "";
    string ce = "";

    // process options

    while ((option_char = getopt()) != EOF)
        switch (option_char) {
        case 'p':
            parse = true;
            name = getopt.optarg;
            break;

        case 's':
        	send = true;
        	name = getopt.optarg;
        	break;

        case 't':
        	trans = true;
        	name = getopt.optarg;
        	break;

        case 'i':
        	intern = true;
        	name = getopt.optarg;
        	break;

        case 'c':
        	constrained = true;
        	ce = getopt.optarg;
        	break;

        case 'd':
            debug = true;
            break;

        case 'D':
            ce_parser_debug = true;
            break;

        case 'x':
            print = true;
            break;

        case 'e':
        	series_values = true;
        	break;

        case '?':
        case 'h':
            usage();
            return 0;

        default:
            cerr << "Error: ";
            usage();
            return 1;
        }

    if (! (parse || send || trans || intern)) {
        cerr << "Error: ";
        usage();
        return 1;
    }

    try {
        if (parse) {
            DMR *dmr = test_dap4_parser(name, debug, print);
            delete dmr;
        }
        // Add constraint and series values when ready
        if (send) {
        	DMR *dmr = test_dap4_parser(name, debug, print);

        	string file_name = send_data(dmr, ce, series_values, ce_parser_debug);
        	if (print)
        		cout << "Response file: " << file_name << endl;
        	delete dmr;
        }

        if (trans) {
        	DMR *dmr = test_dap4_parser(name, debug, print);
        	string file_name = send_data(dmr, ce, series_values, ce_parser_debug);
         	delete dmr;

        	DMR *client = read_data_plain(file_name, debug);

        	if (print) {
        		XMLWriter xml;
        		// received data never have send_p set; don't set 'constrained'
        		client->print_dap4(xml, false /* constrained */);
        		cout << xml.get_doc() << endl;

				cout << "The data:" << endl;
        	}

        	// if trans is used, the data are printed regardless of print's value
    		client->root()->print_val(cout, "", false);
    		cout << endl;

        	delete client;
        }

        if (intern) {
        	DMR *dmr = test_dap4_parser(name, debug, print);
        	intern_data(dmr, "", series_values);

        	if (print) {
        		XMLWriter xml;
        		dmr->print_dap4(xml, false /*constrained*/);
        		cout << xml.get_doc() << endl;

				cout << "The data:" << endl;
        	}

        	// if trans is used, the data are printed regardless of print's value
    		dmr->root()->print_val(cout, "", false);
    		cout << endl;

        	delete dmr;
        }
    }
    catch (Error &e) {
        cerr << "Error: " << e.get_error_message() << endl;
        return 1;
    }

    return 0;
}
