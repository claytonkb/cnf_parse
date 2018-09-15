// cnf_parse.h
//

#pragma once

#include "cutils.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <iostream>
#include <typeinfo>
#include <fstream>
#include <istream>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>
#include <map>

#define streq(x,y)  ( strcmp(x,y) == 0 )

struct cstring_cmp {
    bool operator () (const char* a, const char* b) const {
        return strcmp(a,b)<0;
    } 
};


typedef struct {
    int    *variables;
    int    *clauses;
    int     num_variables;
    int     num_clauses;
    int     num_assignments;
} clause_list;


clause_list *parse_DIMACS(const char *dimacs_str);
int read_format(char *dimacs_str, clause_list *cl);
int read_clauses(char *dimacs_str, clause_list *cl);
int skip_comments(char *dimacs_str);
int skip_ws(char *dimacs_str);


// Clayton Bauman 2018

