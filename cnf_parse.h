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
#define first_char_dot(x) ((x).find(".",0,1))
#define not_section_label(x) (((x) == string::npos) || ((x) != 0))

struct cstring_cmp {
    bool operator () (const char* a, const char* b) const {
        return strcmp(a,b)<0;
    } 
};


// Clayton Bauman 2018

