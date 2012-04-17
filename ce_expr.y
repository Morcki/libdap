
// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2002,2003 OPeNDAP, Inc.
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
 
// (c) COPYRIGHT URI/MIT 1995-1999
// Please read the full copyright statement in the file COPYRIGHT_URI.
//
// Authors:
//      jhrg,jimg       James Gallagher <jgallagher@gso.uri.edu>

/*
  This is the parser for the DODS constraint expression grammar. The parser
  calls various `helper' functions defined by the DAP classes which either
  implement the operations (in the case of relational ops) or store
  information (in the case of selection operations). 

  jhrg 9/5/95
*/

%{

#include "config.h"

static char rcsid[] not_used = {"$Id$"};

#include <cassert>
#include <cstdlib>
#include <string>
#include <cstring>
#include <stack>

#include "debug.h"
#include "escaping.h"

#include "DDS.h"
#include "ConstraintEvaluator.h"

#include "BaseType.h"

#include "Byte.h"
#include "Int16.h"
#include "UInt16.h"
#include "Int32.h"
#include "UInt32.h"
#include "Float32.h"
#include "Float64.h"
#include "Str.h"
#include "Url.h"
#include "Array.h"
#include "Structure.h"
#include "Sequence.h"
#include "Grid.h"

#include "Error.h"

#include "util.h"
#include "parser.h"
#include "ce_parser.h"
#include "expr.h"
#include "RValue.h"

using std::cerr;
using std::endl;
using namespace libdap ;

#define EVALUATOR(arg) (static_cast<ce_parser_arg*>(arg)->get_eval())
#define DDS(arg) (static_cast<ce_parser_arg*>(arg)->get_dds())

#define YYPARSE_PARAM arg

int ce_exprlex(void);		/* the scanner; see expr.lex */

void ce_exprerror(const char *s);	/* easier to overload than to use stdarg... */
void ce_exprerror(const char *s, const char *s2);
void no_such_func(char *name);
void no_such_ident(char *name, char *word);

void ce_exprerror(const string &s); 
void ce_exprerror(const string &s, const string &s2);
void no_such_func(const string &name);
void no_such_ident(const string &name, const string &word);

int_list *make_array_index(value &i1, value &i2, value &i3);
int_list *make_array_index(value &i1, value &i2);
int_list *make_array_index(value &i1);
int_list_list *make_array_indices(int_list *index);
int_list_list *append_array_index(int_list_list *indices, int_list *index);

void delete_array_indices(int_list_list *indices);
bool bracket_projection(DDS &table, const char *name, 
			int_list_list *indices);

void process_array_indices(BaseType *variable, int_list_list *indices); 
void process_grid_indices(BaseType *variable, int_list_list *indices); 
void process_sequence_indices(BaseType *variable, int_list_list *indices);

/* Replace these with method calls. jhrg 8/31/06 */
bool is_array_t(BaseType *variable);
bool is_grid_t(BaseType *variable);
bool is_sequence_t(BaseType *variable);

BaseType *make_variable(ConstraintEvaluator &eval, const value &val);
bool_func get_function(const ConstraintEvaluator &eval, const char *name);
btp_func get_btp_function(const ConstraintEvaluator &eval, const char *name);
proj_func get_proj_function(const ConstraintEvaluator &eval, const char *name);

%}

%union {
    bool boolean;
    int op;
    char id[ID_MAX];

    libdap::value val;               // value is defined in expr.h

    libdap::bool_func b_func;
    libdap::btp_func bt_func;

    libdap::int_list *int_l_ptr;
    libdap::int_list_list *int_ll_ptr;
    
    libdap::rvalue *rval_ptr;
    libdap::rvalue_list *r_val_l_ptr;
}

%token <val> SCAN_STR

%token <id> SCAN_WORD

%token <op> SCAN_EQUAL
%token <op> SCAN_NOT_EQUAL
%token <op> SCAN_GREATER
%token <op> SCAN_GREATER_EQL
%token <op> SCAN_LESS
%token <op> SCAN_LESS_EQL
%token <op> SCAN_REGEXP

%type <boolean> constraint_expr projection proj_clause proj_function
%type <boolean> array_projection selection clause bool_function
%type <id> array_proj_clause name
%type <op> rel_op
%type <int_l_ptr> array_index
%type <int_ll_ptr> array_indices
%type <rval_ptr> r_value id_or_const
%type <r_val_l_ptr> r_value_list arg_list

%%

constraint_expr: /* empty constraint --> send all */
                 {
		     DDS(arg)->mark_all(true);
		     $$ = true;
		 }
                 /* projection only */
                 | projection
		 /* selection only --> project everything */
                 | '&' { DDS(arg)->mark_all(true); } selection
                 { 
		     $$ = $3;
		 }
                 | projection '&' selection
                 {
		     $$ = $1 && $3;
		 }
;

projection:     proj_clause
                | proj_clause ',' projection
                {
		    $$ = $1 && $3;
		}
;

proj_clause:	name 
                { 
		    BaseType *var = DDS(arg)->var($1);
		    if (var) {
			DBG(cerr << "Marking " << $1 << endl);
			$$ = DDS(arg)->mark($1, true);
			DBG(cerr << "result: " << $$ << endl);
		    }
		    else {
			no_such_ident($1, "identifier");
		    }
		}
/*		| SCAN_STR
                {
                    if ($1.type != dods_str_c || $1.v.s == 0 || $1.v.s->empty())
                        ce_exprerror("Malformed string", "");
                        
                    BaseType *var = DDS(arg)->var(*($1.v.s));
                    if (var) {
                        DBG(cerr << "Marking " << $1 << endl);
                        $$ = DDS(arg)->mark(*($1.v.s), true);
                        DBG(cerr << "result: " << $$ << endl);
                    }
                    else {
                        no_such_ident(*($1.v.s), "identifier");
                    }
                }
*/
                | proj_function
                {
		    $$ = $1;
		}
		| array_projection
                {
		    $$ = $1;
		}
;

proj_function:  SCAN_WORD '(' arg_list ')'
	        {
		    proj_func p_f = 0;
		    btp_func f = 0;

		    if ((f = get_btp_function(*(EVALUATOR(arg)), $1))) {
			EVALUATOR(arg)->append_clause(f, $3);
			$$ = true;
		    }
		    else if ((p_f = get_proj_function(*(EVALUATOR(arg)), $1))) {
		        DDS &dds = dynamic_cast<DDS&>(*(DDS(arg)));
			BaseType **args = build_btp_args( $3, dds );
			(*p_f)(($3) ? $3->size():0, args, dds, *(EVALUATOR(arg)));
			delete[] args;
			$$ = true;
		    }
		    else {
			no_such_func($1);
		    }
		}
;

selection:	clause
		| selection '&' clause
                {
		    $$ = $1 && $3;
		}
;

clause:		r_value rel_op '{' r_value_list '}'
                {
		    if ($1) {
			EVALUATOR(arg)->append_clause($2, $1, $4);
			$$ = true;
		    }
		    else
			$$ = false;
		}
		| r_value rel_op r_value
                {
		    if ($1) {
			rvalue_list *rv = new rvalue_list;
			rv->push_back($3);
			EVALUATOR(arg)->append_clause($2, $1, rv);
			$$ = true;
		    }
		    else
			$$ = false;
		}
		| bool_function
                {
		    $$ = $1;
		}
;

bool_function: SCAN_WORD '(' arg_list ')'
	       {
		   bool_func b_func = get_function((*EVALUATOR(arg)), $1);
		   if (!b_func) {
		       no_such_func($1);
		   }
		   else {
		       EVALUATOR(arg)->append_clause(b_func, $3);
		       $$ = true;
		   }
	       }
;

r_value:        id_or_const
		| SCAN_WORD '(' arg_list ')'
		{
		    btp_func func = get_btp_function((*EVALUATOR(arg)), $1);
		    if (func) {
			$$ = new rvalue(func, $3);
		    } 
		    else {  		
			no_such_func($1);
		    }
		}
;

r_value_list:	r_value
		{
		    if ($1)
			$$ = make_rvalue_list($1);
		    else
			$$ = 0;
		}
		| r_value_list ',' r_value
                {
		    if ($1 && $3)
			$$ = append_rvalue_list($1, $3);
		    else
			$$ = 0;
		}
;

arg_list:     r_value_list
              {  
		  $$ = $1;
	      }
              | /* Null, argument lists may be empty */
              { 
		  $$ = 0; 
	      }
;

id_or_const:    SCAN_WORD
		{ 
		    BaseType *btp = DDS(arg)->var(www2id(string($1)));
		    if (btp) {
                        btp->set_in_selection(true);
                        $$ = new rvalue(btp);
		    }
		    else {
			value new_val;
			if (check_int32($1)) {
			    new_val.type = dods_int32_c;
			    new_val.v.i = atoi($1);
			}
			else if (check_uint32($1)) {
			    new_val.type = dods_uint32_c;
			    new_val.v.ui = atoi($1);
			}
			else if (check_float64($1)) {
			    new_val.type = dods_float64_c;
			    new_val.v.f = atof($1);
			}
			else {
			    new_val.type = dods_str_c;
			    new_val.v.s = new string($1);
			}
			BaseType *btp = make_variable((*EVALUATOR(arg)), new_val); 
			// *** test for btp == null
			// delete new_val.v.s; // Str::val2buf copies the value.
			$$ = new rvalue(btp);
		    }
		}
                | SCAN_STR
                {
                    if ($1.type != dods_str_c || $1.v.s == 0 || $1.v.s->empty())
                        ce_exprerror("Malformed string", "");
                        
                    BaseType *var = DDS(arg)->var(*($1.v.s));
                    if (var) {
                        $$ = new rvalue(var);
                    }
                    else {
                        var = make_variable((*EVALUATOR(arg)), $1); 
                        $$ = new rvalue(var);
                    }
                }
;

array_projection : array_proj_clause
                {
                    $$ = (*DDS(arg)).mark($1, true);
                }
                
array_proj_clause: name array_indices
                {
                    if (!bracket_projection((*DDS(arg)), $1, $2))
                      no_such_ident($1, "array, grid or sequence");

                    strncpy($$, $1, ID_MAX-1);
                    $$[ID_MAX-1] = '\0';
                }
                | array_proj_clause name
                {
                    string name = string($1) + string ($2);
                    strncpy($$, name.c_str(), ID_MAX-1);
                    $$[ID_MAX-1] = '\0';
                }
                | array_proj_clause name array_indices
                {
                    string name = string($1) + string ($2);
                    if (!bracket_projection((*DDS(arg)), name.c_str(), $3))
                      no_such_ident(name.c_str(), "array, grid or sequence");

                    strncpy($$, name.c_str(), ID_MAX-1);
                    $$[ID_MAX-1] = '\0';
                }
;

name:           SCAN_WORD
                | SCAN_STR
                {
                    if ($1.type != dods_str_c || $1.v.s == 0 || $1.v.s->empty())
                        ce_exprerror("Malformed string", "");
                        
                    strncpy($$, www2id(*($1.v.s)).c_str(), ID_MAX-1);
                    // delete the string? ***
                    $$[ID_MAX-1] = '\0';
/*
                    BaseType *var = DDS(arg)->var(*($1.v.s));
                    if (var) {
                        DBG(cerr << "Marking " << $1 << endl);
                        $$ = DDS(arg)->mark(*($1.v.s), true);
                        DBG(cerr << "result: " << $$ << endl);
                    }
                    else {
                        no_such_ident(*($1.v.s), "identifier");
                    }
*/
                }
;

array_indices:  array_index
                {
		    $$ = make_array_indices($1);
		}
                | array_indices array_index
                {
		    $$ = append_array_index($1, $2);
		}
;

array_index: 	'[' SCAN_WORD ']'
                {
		    if (!check_uint32($2)) {
			string msg = "The word `";
			msg += string($2) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    value i;
		    i.type = dods_uint32_c;
		    i.v.i = atoi($2);
		    $$ = make_array_index(i);
		}
		|'[' SCAN_WORD ':' SCAN_WORD ']'
                {
		    if (!check_uint32($2)) {
			string msg = "The word `";
			msg += string($2) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    if (!check_uint32($4)) {
			string msg = "The word `";
			msg += string($4) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    value i,j;
		    i.type = j.type = dods_uint32_c;
		    i.v.i = atoi($2);
		    j.v.i = atoi($4);
		    $$ = make_array_index(i, j);
		}
		| '[' SCAN_WORD ':' SCAN_WORD ':' SCAN_WORD ']'
                {
		    if (!check_uint32($2)) {
			string msg = "The word `";
			msg += string($2) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    if (!check_uint32($4)) {
			string msg = "The word `";
			msg += string($4) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    if (!check_uint32($6)) {
			string msg = "The word `";
			msg += string($6) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    value i, j, k;
		    i.type = j.type = k.type = dods_uint32_c;
		    i.v.i = atoi($2);
		    j.v.i = atoi($4);
		    k.v.i = atoi($6);
		    $$ = make_array_index(i, j, k);
		}
;

rel_op:		SCAN_EQUAL
		| SCAN_NOT_EQUAL
		| SCAN_GREATER
		| SCAN_GREATER_EQL
		| SCAN_LESS
		| SCAN_LESS_EQL
		| SCAN_REGEXP
;

%%

// All these error reporting function now throw instances of Error. The expr
// parser no longer returns an error code to indicate and error. 2/16/2000
// jhrg.

void
ce_exprerror(const string &s)
{
    ce_exprerror(s.c_str());
}

void ce_exprerror(const char *s)
{
    // cerr << "Expression parse error: " << s << endl;
    string msg = "Constraint expression parse error: " + (string) s;
    throw Error(malformed_expr, msg);
}

void ce_exprerror(const string &s, const string &s2)
{
    ce_exprerror(s.c_str(), s2.c_str());
}

void ce_exprerror(const char *s, const char *s2)
{
    string msg = "Constraint expression parse error: " + (string) s + ": " + (string) s2;
    throw Error(malformed_expr, msg);
}

void no_such_ident(const string &name, const string &word)
{
    string msg = "No such " + word + " in dataset";
    ce_exprerror(msg.c_str(), name);
}

void no_such_ident(char *name, char *word)
{
    string msg = "No such " + (string) word + " in dataset";
    ce_exprerror(msg.c_str(), name);
}

void no_such_func(const string &name)
{
    no_such_func(name.c_str());
}

void no_such_func(char *name)
{
    ce_exprerror("Not a registered function", name);
}

/* If we're calling this, assume var is not a Sequence. But assume that the
 name contains a dot and it's a separator. Look for the rightmost dot and
 then look to see if the name to the left is a sequence. Return a pointer
 to the sequence if it is otherwise return null. Uses tail-recursion to
 'walk' back from right to left looking at each dot. This way the sequence
 will be found even if there are structures between the field and the
 Sequence. */
static Sequence *
parent_is_sequence(DDS &table, const string &n)
{
    string::size_type dotpos = n.find_last_of('.');
    if (dotpos == string::npos)
        return 0;

    string s = n.substr(0, dotpos);

    // If the thing returned by table.var is not a Sequence, this cast
    // will yield null.
    Sequence *seq = dynamic_cast<Sequence*> (table.var(s));
    if (seq)
        return seq;
    else
        return parent_is_sequence(table, s);
}

bool bracket_projection(DDS &table, const char *name, int_list_list *indices)
{
    BaseType *var = table.var(name);
    Sequence *seq; // used in last else-if clause
#if 0
    Array *array;
#endif    
    if (!var)
        return false;

    if (is_array_t(var)) {
        /* calls to set_send_p should be replaced with
         calls to DDS::mark so that arrays of Structures,
         etc. will be processed correctly when individual
         elements are projected using short names.
         9/1/98 jhrg */
        /* var->set_send_p(true); */
        //table.mark(name, true);
        // We don't call mark() here for an array. Instead it is called from
        // within the parser. jhrg 10/10/08
        process_array_indices(var, indices); // throws on error
        delete_array_indices(indices);
    }
    else if (is_grid_t(var)) {
        process_grid_indices(var, indices);
        table.mark(name, true);
        delete_array_indices(indices);
    }
    else if (is_sequence_t(var)) {
        table.mark(name, true);
        process_sequence_indices(var, indices);
        delete_array_indices(indices);
    }
    else if ((seq = parent_is_sequence(table, name))) {
        process_sequence_indices(seq, indices);
        table.mark(name, true);
        delete_array_indices(indices);
    }
    else {
        return false;
    }

    return true;
}

// Given three values (I1, I2, I3), all of which must be integers, build an
// int_list which contains those values.
//
// Returns: A pointer to an int_list of three integers or NULL if any of the
// values are not integers.

int_list *
make_array_index(value &i1, value &i2, value &i3)
{
    if (i1.type != dods_uint32_c || i2.type != dods_uint32_c || i3.type != dods_uint32_c)
        return (int_list *) 0;

    int_list *index = new int_list;

    index->push_back((int) i1.v.i);
    index->push_back((int) i2.v.i);
    index->push_back((int) i3.v.i);

    DBG(cout << "index: ";
            for (int_iter dp = index->begin(); dp != index->end(); dp++)
                cout << (*dp) << " ";
\
            cout << endl);

            return index;
        }

        int_list *
        make_array_index(value &i1, value &i2)
        {
            if (i1.type != dods_uint32_c || i2.type != dods_uint32_c)
                return (int_list *) 0;

            int_list *index = new int_list;

            index->push_back((int) i1.v.i);
            index->push_back(1);
            index->push_back((int) i2.v.i);

            DBG(cout << "index: ";
                    for (int_citer dp = index->begin(); dp != index->end(); dp++)
                        cout << (*dp) << " ";
\
                    cout << endl);

                    return index;
                }

                int_list *
                make_array_index(value &i1)
                {
                    if (i1.type != dods_uint32_c)
                        return (int_list *) 0;

                    int_list *index = new int_list;

                    index->push_back((int) i1.v.i);
                    index->push_back(1);
                    index->push_back((int) i1.v.i);

                    DBG(cout << "index: ";
                            for (int_citer dp = index->begin(); dp != index->end(); dp++)
                                cout << (*dp) << " ";
\
                            cout << endl);

                            return index;
                        }

                        int_list_list *
                        make_array_indices(int_list *index)
                        {
                            int_list_list *indices = new int_list_list;

                            DBG(cout << "index: ";
                                    for (int_citer dp = index->begin(); dp != index->end(); dp++)
                                        cout << (*dp) << " ";
\
                                    cout << endl);

                                    assert(index);
                                    indices->push_back(index);

                                    return indices;
                                }

                                int_list_list *
                                append_array_index(int_list_list *indices, int_list *index)
                                {
                                    assert(indices);
                                    assert(index);

                                    indices->push_back(index);

                                    return indices;
                                }

                                // Delete an array indices list. 

                                void delete_array_indices(int_list_list *indices)
                                {
                                    assert(indices);

                                    for (int_list_citer i = indices->begin(); i != indices->end(); i++) {
                                        int_list *il = *i;
                                        assert(il);
                                        delete il;
                                    }

                                    delete indices;
                                }

                                bool is_array_t(BaseType *variable)
                                {
                                    assert(variable);

                                    if (variable->type() != dods_array_c)
                                        return false;
                                    else
                                        return true;
                                }

                                bool is_grid_t(BaseType *variable)
                                {
                                    assert(variable);

                                    if (variable->type() != dods_grid_c)
                                        return false;
                                    else
                                        return true;
                                }

                                bool is_sequence_t(BaseType *variable)
                                {
                                    assert(variable);

                                    if (variable->type() != dods_sequence_c)
                                        return false;
                                    else
                                        return true;
                                }

                                void process_array_indices(BaseType *variable, int_list_list *indices)
                                {
                                    assert(variable);

                                    Array *a = dynamic_cast<Array *> (variable); // replace with dynamic cast
                                    if (!a)
                                        throw Error(
                                                malformed_expr,
                                                string("The constraint expression evaluator expected an array; ")
                                                        + variable->name() + " is not an array.");

                                    if (a->dimensions(true) != (unsigned) indices->size())
                                        throw Error(
                                                malformed_expr,
                                                string("Error: The number of dimensions in the constraint for ")
                                                        + variable->name() + " must match the number in the array.");

                                    DBG(cerr << "Before clear_constraint:" << endl);
                                    DBG(a->print_decl(cerr, "", true, false, true));

                                    // a->reset_constraint();	// each projection erases the previous one

                                    DBG(cerr << "After reset_constraint:" << endl);
                                    DBG(a->print_decl(cerr, "", true, false, true));

                                    assert(indices);
                                    int_list_citer p = indices->begin();
                                    Array::Dim_iter r = a->dim_begin();
                                    for (; p != indices->end() && r != a->dim_end(); p++, r++) {
                                        int_list *index = *p;
                                        assert(index);

                                        int_citer q = index->begin();
                                        assert(q != index->end());
                                        int start = *q;

                                        q++;
                                        int stride = *q;

                                        q++;
                                        int stop = *q;

                                        q++;
                                        if (q != index->end()) {
                                            throw Error(malformed_expr,
                                                    string("Too many values in index list for ") + a->name() + ".");
                                        }

                                        DBG(
                                                cerr << "process_array_indices: Setting constraint on "\
 << a->name()
                                                        << "[" << start << ":" << stop << "]" << endl);

                                        // It's possible that an array will appear more than once in a CE
                                        // (for example, if an array of structures is constrained so that
                                        // only two fields are projected and there's an associated hyperslab).
                                        // However, in this case the two hyperslabs must be equal; test for
                                        // that here. But see clear_constraint above... 10/28/08 jhrg

                                        if (a->send_p() && (a->dimension_start(r, true) != start || a->dimension_stop(
                                                r, true) != stop || a->dimension_stride(r, true) != stride))
                                            throw Error(
                                                    malformed_expr,
                                                    string(
                                                            "The Array was already projected differently in the constraint expression: ")
                                                            + a->name() + ".");

                                        a->add_constraint(r, start, stride, stop);

                                        DBG(cerr << "Set Constraint: " << a->dimension_size(r, true) << endl);
                                    }

                                    DBG(cerr << "After processing loop:" << endl);
                                    DBG(a->print_decl(cerr, "", true, false, true));

                                    DBG(cout << "Array Constraint: ";
                                            for (Array::Dim_iter dp = a->dim_begin(); dp != a->dim_end(); dp++)
                                                cout << a->dimension_size(dp, true) << " ";
\
                                            cout << endl);

                                            if (p != indices->end() && r == a->dim_end()) {
                                                throw Error(malformed_expr,
                                                        string("Too many indices in constraint for ") + a->name() + ".");
                                            }
                                        }

                                        void process_grid_indices(BaseType *variable, int_list_list *indices)
                                        {
                                            assert(variable);
                                            assert(variable->type() == dods_grid_c);
                                            Grid *g = dynamic_cast<Grid *> (variable);
                                            if (!g)
                                                throw Error(unknown_error, "Expected a Grid variable");

                                            Array *a = g->get_array();
#if 0
                                            // This test s now part of Grid::get_array(). jhrg 3/5/09
                                            if (!a)
                                            throw InternalErr(__FILE__, __LINE__, "Malformed Grid variable");
#endif
                                            if (a->dimensions(true) != (unsigned) indices->size())
                                                throw Error(
                                                        malformed_expr,
                                                        string("Error: The number of dimensions in the constraint for ")
                                                                + variable->name()
                                                                + " must match the number in the grid.");

                                            // First do the constraints on the ARRAY in the grid.
                                            process_array_indices(g->array_var(), indices);

                                            // Now process the maps.
                                            Grid::Map_iter r = g->map_begin();

                                            // Suppress all maps by default.
                                            for (; r != g->map_end(); r++) {
                                                (*r)->set_send_p(false);
                                            }

                                            // Add specified maps to the current projection.
                                            assert(indices);
                                            int_list_citer p = indices->begin();
                                            r = g->map_begin();
                                            for (; p != indices->end() && r != g->map_end(); p++, r++) {
                                                int_list *index = *p;
                                                assert(index);

                                                int_citer q = index->begin();
                                                assert(q != index->end());
                                                int start = *q;

                                                q++;
                                                int stride = *q;

                                                q++;
                                                int stop = *q;

                                                BaseType *btp = *r;
                                                assert(btp);
                                                assert(btp->type() == dods_array_c);
                                                Array *a = (Array *) btp;
                                                a->set_send_p(true);
                                                a->reset_constraint();

                                                q++;
                                                if (q != index->end()) {
                                                    throw Error(
                                                            malformed_expr,
                                                            string("Too many values in index list for ") + a->name()
                                                                    + ".");
                                                }

                                                DBG(
                                                        cerr << "process_grid_indices: Setting constraint on "\

                                                                << a->name() << "[" << start << ":" << stop << "]"
                                                                << endl);

                                                Array::Dim_iter si = a->dim_begin();
                                                a->add_constraint(si, start, stride, stop);

DBG                                            (Array::Dim_iter aiter = a->dim_begin();
                                                    cerr << "Set Constraint: "
                                                    << a->dimension_size(aiter, true) << endl);
                                        }

                                        DBG(cout << "Grid Constraint: ";
                                                for (Array::Dim_iter dp = ((Array *)g->array_var())->dim_begin();
                                                        dp != ((Array *)g->array_var())->dim_end();
                                                        dp++)
                                                cout << ((Array *)g->array_var())->dimension_size(dp, true) << " ";
                                                cout << endl);

                                        if (p!=indices->end() && r==g->map_end()) {
                                            throw Error(malformed_expr,
                                                    string("Too many indices in constraint for ")
                                                    + (*r)->name() + ".");
                                        }
                                    }

void process_sequence_indices(BaseType *variable, int_list_list *indices)
{
    assert(variable);
    assert(variable->type() == dods_sequence_c);
    Sequence *s = dynamic_cast<Sequence *> (variable);
    if (!s)
        throw Error(malformed_expr, "Expected a Sequence variable");

    // Add specified maps to the current projection.
    assert(indices);
    for (int_list_citer p = indices->begin(); p != indices->end(); p++) {
        int_list *index = *p;
        assert(index);

        int_citer q = index->begin();
        assert(q != index->end());
        int start = *q;

        q++;
        int stride = *q;

        q++;
        int stop = *q;

        q++;
        if (q != index->end()) {
            throw Error(malformed_expr, string("Too many values in index list for ") + s->name() + ".");
        }

        s->set_row_number_constraint(start, stop, stride);
    }
}

// Given a value, wrap it up in a BaseType and return a pointer to the same.

BaseType *
make_variable(ConstraintEvaluator &eval, const value &val)
{
    BaseType *var;
    switch (val.type) {
    case dods_int32_c: {
        var = new Int32("dummy");
        var->val2buf((void *) &val.v.i);
        break;
    }

    case dods_float64_c: {
        var = new Float64("dummy");
        var->val2buf((void *) &val.v.f);
        break;
    }

    case dods_str_c: {
        var = new Str("dummy");
        var->val2buf((void *) val.v.s);
        break;
    }

    default:
        var = (BaseType *) 0;
        return var;
    }

    var->set_read_p(true); // ...so the evaluator will know it has data
    eval.append_constant(var);

    return var;
}

// Given a string (passed in VAL), consult the DDS CE function lookup table
// to see if a function by that name exists. 
// NB: function arguments are type-checked at run-time.
//
// Returns: A pointer to the function or NULL if not such function exists.

bool_func get_function(const ConstraintEvaluator &eval, const char *name)
{
    bool_func f;

    if (eval.find_function(name, &f))
        return f;
    else
        return 0;
}

btp_func get_btp_function(const ConstraintEvaluator &eval, const char *name)
{
    btp_func f;

    if (eval.find_function(name, &f))
        return f;
    else
        return 0;
}

proj_func get_proj_function(const ConstraintEvaluator &eval, const char *name)
{
    proj_func f;

    if (eval.find_function(name, &f))
        return f;
    else
        return 0;
}

