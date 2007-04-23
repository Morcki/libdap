typedef union {
#ifdef __SUNPRO_CC
    int boolean;
#else
    bool boolean;
#endif
    int integer;
    char *string;
} YYSTYPE;
#define	SCAN_INT	257
#define	SCAN_STR	258
#define	SCAN_ERROR	259
#define	SCAN_CODE	260
#define	SCAN_MSG	261


extern YYSTYPE Errorlval;
