// cnf_parse.cpp
// g++ -std=c++11 -o cnf_parse cnf_parse.cpp numeric.cpp -lm

#include "cnf_parse.h"
#include "cutils.h"

using namespace std;

// c ...
// ...
// p cnf var cls
// <int> ... <int> 0
// ...
// <int> ... <int> 0
clause_list *parse_DIMACS(const char *dimacs_str){

    char *parse_str = (char*)malloc(strlen(dimacs_str)+1);
    strcpy(parse_str,dimacs_str);

    clause_list *cl = (clause_list*)malloc(sizeof(clause_list));
    cl->num_clauses=0;
    cl->num_variables=0;

    parse_str += skip_comments(parse_str);

    parse_str += read_format(parse_str, cl);

    read_clauses(parse_str, cl);

    // skip_ws(parse_str);
    // check we're at EOF (warn, don't error)

    return cl;

}


// p <format> <var> <cls>
//      format=cnf
//      vars=number of variables
//      cls=number of clauses
int read_format(char *dimacs_str, clause_list *cl){

    char *token = strtok(dimacs_str, " ");
    int var_int;

    char *remainder;

    if(!streq(token,"p")){
        _fatal("Expected 'p' line");
    }

    token = strtok(NULL, " ");

    if(!streq(token,"cnf")){
        _fatal("Expected token 'cnf'");
    }

    token = strtok(NULL, " ");
    var_int = atoi(token);
    cl->num_variables = var_int;

    token = strtok(NULL, " \n");
    var_int = atoi(token);
    cl->num_clauses = var_int;

    remainder = (token + (strlen(token)+1));

    return remainder-dimacs_str;

}


// assumes cl->num_clauses and cl->num_variables are valid
//
int read_clauses(char *dimacs_str, clause_list *cl){

    int clause_ctr   = 0;
    int assignment_ctr = 0;

    char *token;
    int   var_int;

    bool init_tok = true;

    int *clauses   = (int*)malloc(sizeof(int)*cl->num_clauses);
    int *variables = (int*)malloc(sizeof(int)*CLAUSE_K*cl->num_clauses);

    while(clause_ctr < cl->num_clauses){

        var_int=1;

        clauses[clause_ctr] = assignment_ctr;
        clause_ctr++;

        while(var_int && (assignment_ctr < CLAUSE_K*cl->num_clauses)){

            if(init_tok){
                token = strtok(dimacs_str, " \n");
                init_tok=false;
            }
            else{
                token = strtok(NULL, " \n");
            }

            if(token == NULL){
                _fatal("Unexpected EOF");
            }

            var_int = atoi(token);

            if(var_int){
                variables[assignment_ctr] = var_int;
                assignment_ctr++;
            }

        }

    }

//    remainder = (token + (strlen(token)+1));

    // this should never break unless you have tons of bizarrely huge clauses
    // if it does, increase CLAUSE_K and recompile
    if(assignment_ctr >= CLAUSE_K*cl->num_clauses)
        _fatal("CNF_PARSE couldn't process this CNF file");

    cl->num_assignments = assignment_ctr;
    cl->clauses = clauses;

    if(assignment_ctr){
        cl->variables = (int*)malloc(sizeof(int)*assignment_ctr);
        memcpy(cl->variables, variables, sizeof(int)*assignment_ctr);
        free(variables);
    }
    else{
        cl->variables = NULL;
    }

}



//
//
int skip_comments(char *dimacs_str){

    int skipped=0;

    while(dimacs_str[skipped] == 'c'){

        skipped++;

        while(dimacs_str[skipped] != 0xa)
            skipped++;

        while(dimacs_str[skipped] == 0xa)
            skipped++;

    }

    return skipped;

}


//
//
int skip_ws(char *dimacs_str){

    int skipped=0;

    while(   dimacs_str[skipped] == ' '
          || dimacs_str[skipped] == '\t'
          || dimacs_str[skipped] == 0xa
          || dimacs_str[skipped] == 0xd)
        skipped++;

    return skipped;

}

// Clayton Bauman 2018

