
/*  A Bison parser, made from ce_expr.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse ce_exprparse
#define yylex ce_exprlex
#define yyerror ce_exprerror
#define yylval ce_exprlval
#define yychar ce_exprchar
#define yydebug ce_exprdebug
#define yynerrs ce_exprnerrs
#define	SCAN_STR	257
#define	SCAN_WORD	258
#define	SCAN_EQUAL	259
#define	SCAN_NOT_EQUAL	260
#define	SCAN_GREATER	261
#define	SCAN_GREATER_EQL	262
#define	SCAN_LESS	263
#define	SCAN_LESS_EQL	264
#define	SCAN_REGEXP	265

#line 41 "ce_expr.y"


#include "config.h"

static char rcsid[] not_used = {"$Id$"};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <string>

#include "debug.h"
#include "escaping.h"

#include "DDS.h"
#include "ConstraintEvaluator.h"

#include "BaseType.h"
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

bool process_array_indices(BaseType *variable, int_list_list *indices); 
bool process_grid_indices(BaseType *variable, int_list_list *indices); 
bool process_sequence_indices(BaseType *variable, int_list_list *indices);

/* Replace these with method calls. jhrg 8/31/06 */
bool is_array_t(BaseType *variable);
bool is_grid_t(BaseType *variable);
bool is_sequence_t(BaseType *variable);

#if 0
/* Moved to RValue jhrg 8/31/06 */

rvalue_list *make_rvalue_list(rvalue *rv);
rvalue_list *append_rvalue_list(rvalue_list *rvals, rvalue *rv);
#endif

BaseType *make_variable(DDS &table, ConstraintEvaluator &eval, const value &val);
bool_func get_function(const ConstraintEvaluator &eval, const char *name);
btp_func get_btp_function(const ConstraintEvaluator &eval, const char *name);
proj_func get_proj_function(const ConstraintEvaluator &eval, const char *name);


#line 126 "ce_expr.y"
typedef union {
    bool boolean;
    int op;
    char id[ID_MAX];

    value val;

    bool_func b_func;
    btp_func bt_func;

    int_list *int_l_ptr;
    int_list_list *int_ll_ptr;
    
    rvalue *rval_ptr;
    rvalue_list *r_val_l_ptr;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		61
#define	YYFLAG		-32768
#define	YYNTBASE	21

#define YYTRANSLATE(x) ((unsigned)(x) <= 265 ? yytranslate[x] : 37)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    12,     2,    14,
    15,     2,     2,    13,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    20,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    18,     2,    19,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    16,     2,    17,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     3,     4,     8,    12,    14,    18,    20,    22,
    24,    29,    31,    35,    41,    45,    47,    52,    54,    59,
    61,    65,    67,    68,    70,    72,    75,    77,    80,    84,
    90,    98,   100,   102,   104,   106,   108,   110
};

static const short yyrhs[] = {    -1,
    23,     0,     0,    12,    22,    26,     0,    23,    12,    26,
     0,    24,     0,    24,    13,    23,     0,     4,     0,    25,
     0,    33,     0,     4,    14,    31,    15,     0,    27,     0,
    26,    12,    27,     0,    29,    36,    16,    30,    17,     0,
    29,    36,    29,     0,    28,     0,     4,    14,    31,    15,
     0,    32,     0,     4,    14,    31,    15,     0,    29,     0,
    30,    13,    29,     0,    30,     0,     0,     4,     0,     3,
     0,     4,    34,     0,    35,     0,    34,    35,     0,    18,
     4,    19,     0,    18,     4,    20,     4,    19,     0,    18,
     4,    20,     4,    20,     4,    19,     0,     5,     0,     6,
     0,     7,     0,     8,     0,     9,     0,    10,     0,    11,
     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   165,   171,   173,   174,   177,   183,   184,   190,   202,   206,
   212,   233,   234,   240,   249,   260,   266,   279,   290,   302,
   309,   318,   322,   328,   359,   366,   376,   380,   386,   398,
   416,   442,   443,   444,   445,   446,   447,   448
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","SCAN_STR",
"SCAN_WORD","SCAN_EQUAL","SCAN_NOT_EQUAL","SCAN_GREATER","SCAN_GREATER_EQL",
"SCAN_LESS","SCAN_LESS_EQL","SCAN_REGEXP","'&'","','","'('","')'","'{'","'}'",
"'['","']'","':'","constraint_expr","@1","projection","proj_clause","proj_function",
"selection","clause","bool_function","r_value","r_value_list","arg_list","id_or_const",
"array_proj","array_indices","array_index","rel_op", NULL
};
#endif

static const short yyr1[] = {     0,
    21,    21,    22,    21,    21,    23,    23,    24,    24,    24,
    25,    26,    26,    27,    27,    27,    28,    29,    29,    30,
    30,    31,    31,    32,    32,    33,    34,    34,    35,    35,
    35,    36,    36,    36,    36,    36,    36,    36
};

static const short yyr2[] = {     0,
     0,     1,     0,     3,     3,     1,     3,     1,     1,     1,
     4,     1,     3,     5,     3,     1,     4,     1,     4,     1,
     3,     1,     0,     1,     1,     2,     1,     2,     3,     5,
     7,     1,     1,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     1,
     8,     3,     2,     6,     9,    10,    23,     0,    26,    27,
     0,     0,     0,    25,    24,    20,    22,     0,    18,     0,
    28,    24,     4,    12,    16,     0,     5,     7,    23,     0,
    11,    29,     0,    23,     0,    32,    33,    34,    35,    36,
    37,    38,     0,     0,    21,     0,     0,    13,     0,    15,
    19,    30,     0,    19,     0,     0,    14,    31,     0,     0,
     0
};

static const short yydefgoto[] = {    59,
    11,     3,     4,     5,    23,    24,    25,    16,    17,    18,
    19,     6,     9,    10,    43
};

static const short yypact[] = {     1,
    -6,-32768,    -5,    -4,-32768,-32768,    14,    29,    18,-32768,
    17,    17,    33,-32768,    24,-32768,    26,    25,-32768,     3,
-32768,    27,    30,-32768,-32768,    20,    30,-32768,    14,    14,
-32768,-32768,    39,    14,    17,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,    -1,    31,-32768,    15,    32,-32768,    14,-32768,
-32768,-32768,    40,     4,    -3,    34,-32768,-32768,    45,    48,
-32768
};

static const short yypgoto[] = {-32768,
-32768,    36,-32768,-32768,    38,    16,-32768,   -11,     5,   -23,
-32768,-32768,-32768,    43,-32768
};


#define	YYLAST		54


static const short yytable[] = {    26,
    26,    14,    15,   -17,     1,    44,    12,     7,    13,    30,
    47,     8,     2,    57,    49,   -17,    14,    15,    45,    14,
    22,    32,    33,    26,    36,    37,    38,    39,    40,    41,
    42,    50,    20,    52,    53,     8,     1,    29,    30,    31,
    34,    35,    46,    56,    60,    51,    54,    61,    28,    27,
    48,    21,    58,    55
};

static const short yycheck[] = {    11,
    12,     3,     4,     0,     4,    29,    12,    14,    13,    13,
    34,    18,    12,    17,    16,    12,     3,     4,    30,     3,
     4,    19,    20,    35,     5,     6,     7,     8,     9,    10,
    11,    43,     4,    19,    20,    18,     4,    14,    13,    15,
    14,    12,     4,     4,     0,    15,    15,     0,    13,    12,
    35,     9,    19,    49
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 166 "ce_expr.y"
{
		     DDS(arg)->mark_all(true);
		     yyval.boolean = true;
		 ;
    break;}
case 3:
#line 173 "ce_expr.y"
{ DDS(arg)->mark_all(true); ;
    break;}
case 4:
#line 174 "ce_expr.y"
{ 
		     yyval.boolean = yyvsp[0].boolean;
		 ;
    break;}
case 5:
#line 178 "ce_expr.y"
{
		     yyval.boolean = yyvsp[-2].boolean && yyvsp[0].boolean;
		 ;
    break;}
case 7:
#line 185 "ce_expr.y"
{
		    yyval.boolean = yyvsp[-2].boolean && yyvsp[0].boolean;
		;
    break;}
case 8:
#line 191 "ce_expr.y"
{ 
		    BaseType *var = DDS(arg)->var(yyvsp[0].id);
		    if (var) {
			DBG(cerr << "Marking " << yyvsp[0].id << endl);
			yyval.boolean = DDS(arg)->mark(yyvsp[0].id, true);
			DBG(cerr << "result: " << yyval.boolean << endl);
		    }
		    else {
			no_such_ident(yyvsp[0].id, "identifier");
		    }
		;
    break;}
case 9:
#line 203 "ce_expr.y"
{
		    yyval.boolean = yyvsp[0].boolean;
		;
    break;}
case 10:
#line 207 "ce_expr.y"
{
		    yyval.boolean = yyvsp[0].boolean;
		;
    break;}
case 11:
#line 213 "ce_expr.y"
{
		    proj_func p_f = 0;
		    btp_func f = 0;

		    if ((f = get_btp_function(*(EVALUATOR(arg)), yyvsp[-3].id))) {
			EVALUATOR(arg)->append_clause(f, yyvsp[-1].r_val_l_ptr);
			yyval.boolean = true;
		    }
		    else if ((p_f = get_proj_function(*(EVALUATOR(arg)), yyvsp[-3].id))) {
		        DDS &dds = dynamic_cast<DDS&>(*(DDS(arg)));
			BaseType **args = build_btp_args( yyvsp[-1].r_val_l_ptr, dds, dds.get_dataset_name() );
			(*p_f)((yyvsp[-1].r_val_l_ptr) ? yyvsp[-1].r_val_l_ptr->size():0, args, dds, *(EVALUATOR(arg)));
			yyval.boolean = true;
		    }
		    else {
			no_such_func(yyvsp[-3].id);
		    }
		;
    break;}
case 13:
#line 235 "ce_expr.y"
{
		    yyval.boolean = yyvsp[-2].boolean && yyvsp[0].boolean;
		;
    break;}
case 14:
#line 241 "ce_expr.y"
{
		    if (yyvsp[-4].rval_ptr) {
			EVALUATOR(arg)->append_clause(yyvsp[-3].op, yyvsp[-4].rval_ptr, yyvsp[-1].r_val_l_ptr);
			yyval.boolean = true;
		    }
		    else
			yyval.boolean = false;
		;
    break;}
case 15:
#line 250 "ce_expr.y"
{
		    if (yyvsp[-2].rval_ptr) {
			rvalue_list *rv = new rvalue_list;
			rv->push_back(yyvsp[0].rval_ptr);
			EVALUATOR(arg)->append_clause(yyvsp[-1].op, yyvsp[-2].rval_ptr, rv);
			yyval.boolean = true;
		    }
		    else
			yyval.boolean = false;
		;
    break;}
case 16:
#line 261 "ce_expr.y"
{
		    yyval.boolean = yyvsp[0].boolean;
		;
    break;}
case 17:
#line 267 "ce_expr.y"
{
		   bool_func b_func = get_function((*EVALUATOR(arg)), yyvsp[-3].id);
		   if (!b_func) {
		       no_such_func(yyvsp[-3].id);
		   }
		   else {
		       EVALUATOR(arg)->append_clause(b_func, yyvsp[-1].r_val_l_ptr);
		       yyval.boolean = true;
		   }
	       ;
    break;}
case 19:
#line 291 "ce_expr.y"
{
		    btp_func func = get_btp_function((*EVALUATOR(arg)), yyvsp[-3].id);
		    if (func) {
			yyval.rval_ptr = new rvalue(func, yyvsp[-1].r_val_l_ptr);
		    } 
		    else {  		
			no_such_func(yyvsp[-3].id);
		    }
		;
    break;}
case 20:
#line 303 "ce_expr.y"
{
		    if (yyvsp[0].rval_ptr)
			yyval.r_val_l_ptr = make_rvalue_list(yyvsp[0].rval_ptr);
		    else
			yyval.r_val_l_ptr = 0;
		;
    break;}
case 21:
#line 310 "ce_expr.y"
{
		    if (yyvsp[-2].r_val_l_ptr && yyvsp[0].rval_ptr)
			yyval.r_val_l_ptr = append_rvalue_list(yyvsp[-2].r_val_l_ptr, yyvsp[0].rval_ptr);
		    else
			yyval.r_val_l_ptr = 0;
		;
    break;}
case 22:
#line 319 "ce_expr.y"
{  
		  yyval.r_val_l_ptr = yyvsp[0].r_val_l_ptr;
	      ;
    break;}
case 23:
#line 323 "ce_expr.y"
{ 
		  yyval.r_val_l_ptr = 0; 
	      ;
    break;}
case 24:
#line 329 "ce_expr.y"
{ 
		    BaseType *btp = DDS(arg)->var(www2id(string(yyvsp[0].id)));
		    if (!btp) {
			value new_val;
			if (check_int32(yyvsp[0].id)) {
			    new_val.type = dods_int32_c;
			    new_val.v.i = atoi(yyvsp[0].id);
			}
			else if (check_uint32(yyvsp[0].id)) {
			    new_val.type = dods_uint32_c;
			    new_val.v.ui = atoi(yyvsp[0].id);
			}
			else if (check_float64(yyvsp[0].id)) {
			    new_val.type = dods_float64_c;
			    new_val.v.f = atof(yyvsp[0].id);
			}
			else {
			    new_val.type = dods_str_c;
			    new_val.v.s = new string(yyvsp[0].id);
			}
			BaseType *btp = make_variable((*DDS(arg)), (*EVALUATOR(arg)), new_val); 
			// *** test for btp == null
			// delete new_val.v.s; // Str::val2buf copies the value.
			yyval.rval_ptr = new rvalue(btp);
		    }
		    else {
			btp->set_in_selection(true);
			yyval.rval_ptr = new rvalue(btp);
		    }
		;
    break;}
case 25:
#line 360 "ce_expr.y"
{ 
		    BaseType *btp = make_variable((*DDS(arg)), (*EVALUATOR(arg)), yyvsp[0].val); 
		    yyval.rval_ptr = new rvalue(btp);
		;
    break;}
case 26:
#line 367 "ce_expr.y"
{
		  if (!bracket_projection((*DDS(arg)), yyvsp[-1].id, yyvsp[0].int_ll_ptr))
		    // no_such_ident throws an exception.
		    no_such_ident(yyvsp[-1].id, "array, grid or sequence");
		  else
		    yyval.boolean = true;
		;
    break;}
case 27:
#line 377 "ce_expr.y"
{
		    yyval.int_ll_ptr = make_array_indices(yyvsp[0].int_l_ptr);
		;
    break;}
case 28:
#line 381 "ce_expr.y"
{
		    yyval.int_ll_ptr = append_array_index(yyvsp[-1].int_ll_ptr, yyvsp[0].int_l_ptr);
		;
    break;}
case 29:
#line 387 "ce_expr.y"
{
		    if (!check_uint32(yyvsp[-1].id)) {
			string msg = "The word `";
			msg += string(yyvsp[-1].id) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    value i;
		    i.type = dods_uint32_c;
		    i.v.i = atoi(yyvsp[-1].id);
		    yyval.int_l_ptr = make_array_index(i);
		;
    break;}
case 30:
#line 399 "ce_expr.y"
{
		    if (!check_uint32(yyvsp[-3].id)) {
			string msg = "The word `";
			msg += string(yyvsp[-3].id) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    if (!check_uint32(yyvsp[-1].id)) {
			string msg = "The word `";
			msg += string(yyvsp[-1].id) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    value i,j;
		    i.type = j.type = dods_uint32_c;
		    i.v.i = atoi(yyvsp[-3].id);
		    j.v.i = atoi(yyvsp[-1].id);
		    yyval.int_l_ptr = make_array_index(i, j);
		;
    break;}
case 31:
#line 417 "ce_expr.y"
{
		    if (!check_uint32(yyvsp[-5].id)) {
			string msg = "The word `";
			msg += string(yyvsp[-5].id) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    if (!check_uint32(yyvsp[-3].id)) {
			string msg = "The word `";
			msg += string(yyvsp[-3].id) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    if (!check_uint32(yyvsp[-1].id)) {
			string msg = "The word `";
			msg += string(yyvsp[-1].id) + "' is not a valid array index.";
			throw Error(malformed_expr, msg);
		    }
		    value i, j, k;
		    i.type = j.type = k.type = dods_uint32_c;
		    i.v.i = atoi(yyvsp[-5].id);
		    j.v.i = atoi(yyvsp[-3].id);
		    k.v.i = atoi(yyvsp[-1].id);
		    yyval.int_l_ptr = make_array_index(i, j, k);
		;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 451 "ce_expr.y"


// All these error reporting function now throw instnaces of Error. The expr
// parser no longer returns an error code to indicate and error. 2/16/2000
// jhrg.

void
ce_exprerror(const string &s)
{ 
    ce_exprerror(s.c_str());
}

void
ce_exprerror(const char *s)
{
    // cerr << "Expression parse error: " << s << endl;
    string msg = "Constraint expression parse error: " + (string)s;
    throw Error(malformed_expr, msg);
}

void
ce_exprerror(const string &s, const string &s2)
{
    ce_exprerror(s.c_str(), s2.c_str());
}

void
ce_exprerror(const char *s, const char *s2)
{
    string msg = "Constraint expression parse error: " + (string)s + ": " 
	+ (string)s2;
    throw Error(malformed_expr, msg);
}

void
no_such_ident(const string &name, const string &word)
{
    string msg = "No such " + word + " in dataset";
    ce_exprerror(msg.c_str(), name);
}

void
no_such_ident(char *name, char *word)
{
    string msg = "No such " + (string)word + " in dataset";
    ce_exprerror(msg.c_str(), name);
}

void
no_such_func(const string &name)
{
    no_such_func(name.c_str());
}

void
no_such_func(char *name)
{
    ce_exprerror("Not a registered function", name);
}

/* If we're calling this, assume var is not a Sequence. But assume that the
   name contains a dot and it's a separator. Look for the rightmost dot and
   then look to see if the name to the left is a sequence. Return a pointer
   to the sequence if it is otherwise return null. Uses tail-recurrsion to
   'walk' back from right to left looking at each dot. This way the sequence
   will be found even if there are structures between the field and the
   Sequence. */
Sequence *
parent_is_sequence(DDS &table, const char *name)
{
    string n = name;
    string::size_type dotpos = n.find_last_of('.');
    if (dotpos == string::npos)
	return 0;

    string s = n.substr(0, dotpos);
    // If the thing returned by table.var is not a Sequence, this cast
    // will yield null.
    Sequence *seq = dynamic_cast<Sequence*>(table.var(s));
    if (seq)
	return seq;
    else
	return parent_is_sequence(table, s.c_str());
}


bool
bracket_projection(DDS &table, const char *name, int_list_list *indices)
{
    bool status = true;
    BaseType *var = table.var(name);
    Sequence *seq;		// used in last else if clause

    if (!var)
	return false;
	
    if (is_array_t(var)) {
	/* calls to set_send_p should be replaced with
	   calls to DDS::mark so that arrays of Structures,
	   etc. will be processed correctly when individual
	   elements are projected using short names (Whew!)
	   9/1/98 jhrg */
	/* var->set_send_p(true); */
	table.mark(name, true);
	status = process_array_indices(var, indices);
	if (!status) {
	    string msg = "The indices given for `";
	    msg += (string)name + (string)"' are out of range.";
	    throw Error(malformed_expr, msg);
	}
	delete_array_indices(indices);
    }
    else if (is_grid_t(var)) {
	table.mark(name, true);
	/* var->set_send_p(true); */
	status = process_grid_indices(var, indices);
	if (!status) {
	    string msg = "The indices given for `";
	    msg += (string)name + (string)"' are out of range.";
	    throw Error(malformed_expr, msg);
	}
	delete_array_indices(indices);
    }
    else if (is_sequence_t(var)) {
	table.mark(name, true);
	status = process_sequence_indices(var, indices);
	if (!status) {
	    string msg = "The indices given for `";
	    msg += (string)name + (string)"' are out of range.";
	    throw Error(malformed_expr, msg);
	}
	delete_array_indices(indices);
    }
    else if ((seq = parent_is_sequence(table, name))) {
	table.mark(name, true);
	status = process_sequence_indices(seq, indices);
	if (!status) {
	    string msg = "The indices given for `";
	    msg += (string)name + (string)"' are out of range.";
	    throw Error(malformed_expr, msg);
	}
	delete_array_indices(indices);
    }
    else {
	status = false;
    }
  
    return status;
}

// Given three values (I1, I2, I3), all of which must be integers, build an
// int_list which contains those values.
//
// Returns: A pointer to an int_list of three integers or NULL if any of the
// values are not integers.

int_list *
make_array_index(value &i1, value &i2, value &i3)
{
    int_list *index = new int_list;

    if (i1.type != dods_uint32_c
	|| i2.type != dods_uint32_c
	|| i3.type != dods_uint32_c)
	return (int_list *)0;

    index->push_back((int)i1.v.i);
    index->push_back((int)i2.v.i);
    index->push_back((int)i3.v.i);

    DBG(cout << "index: ";\
	for (int_iter dp = index->begin(); dp != index->end(); dp++)\
	cout << (*dp) << " ";\
	cout << endl);

    return index;
}

int_list *
make_array_index(value &i1, value &i2)
{
    int_list *index = new int_list;

    if (i1.type != dods_uint32_c || i2.type != dods_uint32_c)
	return (int_list *)0;

    index->push_back((int)i1.v.i);
    index->push_back(1);
    index->push_back((int)i2.v.i);

    DBG(cout << "index: ";\
	for (int_citer dp = index->begin(); dp != index->end(); dp++)\
	cout << (*dp) << " ";\
	cout << endl);

    return index;
}

int_list *
make_array_index(value &i1)
{
    int_list *index = new int_list;

    if (i1.type != dods_uint32_c)
	return (int_list *)0;

    index->push_back((int)i1.v.i);
    index->push_back(1);
    index->push_back((int)i1.v.i);

    DBG(cout << "index: ";\
	for (int_citer dp = index->begin(); dp != index->end(); dp++)\
	cout << (*dp) << " ";\
	cout << endl);

    return index;
}

int_list_list *
make_array_indices(int_list *index)
{
    int_list_list *indices = new int_list_list;

    DBG(cout << "index: ";\
	for (int_citer dp = index->begin(); dp != index->end(); dp++)\
	cout << (*dp) << " ";\
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

void
delete_array_indices(int_list_list *indices)
{
    assert(indices);

    for (int_list_citer i = indices->begin(); i != indices->end(); i++) {
	int_list *il = *i ;
	assert(il);
	delete il;
    }

    delete indices;
}

bool
is_array_t(BaseType *variable)
{
    assert(variable);

    if (variable->type() != dods_array_c)
	return false;
    else
	return true;
}

bool
is_grid_t(BaseType *variable)
{
    assert(variable);

    if (variable->type() != dods_grid_c)
	return false;
    else
	return true;
}

bool
is_sequence_t(BaseType *variable)
{
    assert(variable);

    if (variable->type() != dods_sequence_c)
	return false;
    else
	return true;
}

bool
process_array_indices(BaseType *variable, int_list_list *indices)
{
    bool status = true;

    assert(variable);

    Array *a = dynamic_cast<Array *>(variable); // replace with dynamic cast
    if (!a)
	throw Error(malformed_expr, 
	   string("The constraint expression evaluator expected an array; ")
		    + variable->name() + " is not an array.");
		   
    if (a->dimensions(true) != (unsigned)indices->size())
	throw Error(malformed_expr, 
	   string("Error: The number of dimenstions in the constraint for ")
		    + variable->name() 
		    + " must match the number in the array.");
		   
    DBG(cerr << "Before clear_costraint:" << endl);
    DBG(a->print_decl(stderr, "", true, false, true));

    a->clear_constraint();	// each projection erases the previous one

    DBG(cerr << "After clear_costraint:" << endl);
    DBG(a->print_decl(stderr, "", true, false, true));

    assert(indices);
    int_list_citer p = indices->begin() ;
    Array::Dim_iter r = a->dim_begin() ;
    for (; p != indices->end() && r != a->dim_end(); p++, r++) {
	int_list *index = *p;
	assert(index);

	int_citer q = index->begin(); 
	assert(q!=index->end());
	int start = *q;

	q++;
	int stride = *q;
	
	q++;
	int stop = *q;

	q++;
	if (q != index->end()) {
	    throw Error(malformed_expr,
			string("Too many values in index list for ")
			+ a->name() + ".");
	}

	DBG(cerr << "process_array_indices: Setting constraint on "\
	    << a->name() << "[" << start << ":" << stop << "]" << endl);

	a->add_constraint(r, start, stride, stop);

	DBG(cerr << "Set Constraint: " << a->dimension_size(r, true) << endl);
    }

    DBG(cerr << "After processing loop:" << endl);
    DBG(a->print_decl(stderr, "", true, false, true));

    DBG(cout << "Array Constraint: ";\
	for (Array::Dim_iter dp = a->dim_begin(); dp != a->dim_end(); dp++)\
	    cout << a->dimension_size(dp, true) << " ";\
	cout << endl);
    
    if (p != indices->end() && r == a->dim_end()) {
	throw Error(malformed_expr,
		    string("Too many indices in constraint for ")
		    + a->name() + ".");
    }

    return status;
}

bool
process_grid_indices(BaseType *variable, int_list_list *indices)
{
    bool status = true;

    assert(variable);
    assert(variable->type() == dods_grid_c);
    Grid *g = dynamic_cast<Grid *>(variable);
    if (!g)
	throw Error(unknown_error, "Expected a Grid variable");

    Array *a = g->get_array();
    if (!a)
	throw InternalErr(__FILE__, __LINE__, "Malformed Grid variable");
    if (a->dimensions(true) != (unsigned)indices->size())
	throw Error(malformed_expr, 
	   string("Error: The number of dimenstions in the constraint for ")
		    + variable->name() 
		    + " must match the number in the grid.");
		   
    // First do the constraints on the ARRAY in the grid.
    process_array_indices(g->array_var(), indices);

    // Now process the maps.
    Grid::Map_iter r = g->map_begin() ;

    // Supress all maps by default.
    for (; r != g->map_end(); r++)
    {
	(*r)->set_send_p(false);
    }

    // Add specified maps to the current projection.
    assert(indices);
    int_list_citer p = indices->begin();
    r = g->map_begin(); 
    for (; p != indices->end() && r != g->map_end(); p++, r++)
    {
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
	Array *a = (Array *)btp;
	a->set_send_p(true);
	a->clear_constraint();

	q++;
	if (q!=index->end()) {
	    throw Error(malformed_expr,
			string("Too many values in index list for ")
			+ a->name() + ".");
	}

	DBG(cerr << "process_grid_indices: Setting constraint on "\
	    << a->name() << "[" << start << ":" << stop << "]" << endl);

	Array::Dim_iter si = a->dim_begin() ;
	a->add_constraint(si, start, stride, stop);

	DBG(Array::Dim_iter aiter = a->dim_begin() ; \
	    cerr << "Set Constraint: " \
	    << a->dimension_size(aiter, true) << endl);
    }

    DBG(cout << "Grid Constraint: ";\
	for (Array::Dim_iter dp = ((Array *)g->array_var())->dim_begin();
	     dp != ((Array *)g->array_var())->dim_end(); \
	     dp++)\
	   cout << ((Array *)g->array_var())->dimension_size(dp, true) << " ";\
	cout << endl);
    
    if (p!=indices->end() && r==g->map_end()) {
	throw Error(malformed_expr,
		    string("Too many indices in constraint for ")
		    + (*r)->name() + ".");
    }

    return status;
}

bool
process_sequence_indices(BaseType *variable, int_list_list *indices)
{
    bool status = true;

    assert(variable);
    assert(variable->type() == dods_sequence_c);
    Sequence *s = dynamic_cast<Sequence *>(variable);
    if (!s)
	throw Error(malformed_expr, "Expected a Sequence variable");

    // Add specified maps to the current projection.
    assert(indices);
    for (int_list_citer p = indices->begin(); p != indices->end(); p++)
    {
	int_list *index = *p;
	assert(index);

	int_citer q = index->begin(); 
	assert(q!=index->end());
	int start = *q;

	q++;
	int stride = *q;
	
	q++;
	int stop = *q;

	q++;
	if (q!=index->end()) {
	  throw Error(malformed_expr, 
		      string("Too many values in index list for ")
		      + s->name() + ".");
	}

	s->set_row_number_constraint(start, stop, stride);
    }

    return status;
}

#if 0
// Create a list of r values and add VAL to the list.
//
// Returns: A pointer to the updated rvalue_list.

/* Moved to RValue. jhrg 8/31/06 */

rvalue_list *
make_rvalue_list(rvalue *rv)
{
    assert(rv);

    rvalue_list *rvals = new rvalue_list;

    return append_rvalue_list(rvals, rv);
}

// Given a rvalue_list pointer RVALS and a value pointer VAL, make a variable
// to hold VAL and append that variable to the list RVALS.
//
// Returns: A pointer to the updated rvalue_list.

rvalue_list *
append_rvalue_list(rvalue_list *rvals, rvalue *rv)
{
    assert(rvals);
    assert(rv);

    rvals->push_back(rv);

    return rvals;
}
#endif

// Given a string which is a URL, dereference it and return the data it
// points to.
#if NO_DEREFERENCE_OP
static rvalue *
dereference_string(string &s)
{
    // FIX Once Connect/HTTPConnect settle down. ***
    unsigned int qpos = s.find('?');
    string url = s.substr(0, qpos);	// strip off CE
    string ce = s.substr(qpos+1);	// yes, get the CE

    Connect c(url); // make the virtual connection

    // the initial URL must be a complete reference to data; thus no
    // additional CE is needed. 
    BaseTypeFactory *factory = new BaseTypeFactory;
    DataDDS *d = new DataDDS(factory);
    c.request_data(*d, ce); 

    // By definition, the DDS `D' can have only one variable, so make sure
    // that is true.
    if (d->num_var() != 1) {
	delete factory; factory = 0;
	delete d; d = 0;
	throw Error (malformed_expr,
		     string("Too many variables in URL; use only single variable projections"));
    }

    // OK, we're here. The first_var() must be the only var, return it bound
    // up in an rvalue struct. NB: the *object* must be copied since the one
    // within DDS `D' will be deleted by D's dtor.
    BaseType *btp = (*(d->var_begin()))->ptr_duplicate();
    rvalue *rv = new rvalue(btp);

    delete factory; factory = 0;
    delete d; d = 0;

    return rv;
}

rvalue *
dereference_url(value &val)
{
    if (val.type != dods_str_c)
	return 0;

    return dereference_string(*val.v.s);
}

// Given a rvalue, get the BaseType that encapsulates its value, make sure it
// is a string and, if all that works, dereference it.

rvalue *
dereference_variable(rvalue *rv, DDS &dds)
{
    assert(rv);
    // the value will be read over the net
    BaseType *btp = rv->bvalue("dummy", dds); 
    if (btp->type() != dods_str_c && btp->type() != dods_url_c) {
	throw Error(malformed_expr, string("The variable `") + btp->name() 
		    + "' must be either a string or a url");
    }

    string s;
    string  *sp = &s;
    btp->buf2val((void **)&sp);
    
    return dereference_string(s);
}
#endif

// Given a value, wrap it up in a BaseType and return a pointer to the same.

BaseType *
make_variable(DDS &table, ConstraintEvaluator &eval, const value &val)
{
    BaseType *var;
    switch (val.type) {
      case dods_int32_c: {
	var = table.get_factory()->NewInt32("dummy");
	var->val2buf((void *)&val.v.i);
	break;
      }

      case dods_float64_c: {
	var = table.get_factory()->NewFloat64("dummy");
	var->val2buf((void *)&val.v.f);
	break;
      }

      case dods_str_c: {
	var = table.get_factory()->NewStr("dummy");
	var->val2buf((void *)val.v.s);
	break;
      }

      default:
	var = (BaseType *)0;
	return var;
    }

    var->set_read_p(true);	// ...so the evaluator will know it has data
    eval.append_constant(var);

    return var;
}

// Given a string (passed in VAL), consult the DDS CE function lookup table
// to see if a function by that name exists. 
// NB: function arguments are type-checked at run-time.
//
// Returns: A poitner to the function or NULL if not such function exists.

bool_func
get_function(const ConstraintEvaluator &eval, const char *name)
{
    bool_func f;

    if (eval.find_function(name, &f))
	return f;
    else
	return 0;
}

btp_func
get_btp_function(const ConstraintEvaluator &eval, const char *name)
{
    btp_func f;

    if (eval.find_function(name, &f))
	return f;
    else
	return 0;
}

proj_func
get_proj_function(const ConstraintEvaluator &eval, const char *name)
{
    proj_func f;

    if (eval.find_function(name, &f))
	return f;
    else
	return 0;
}
