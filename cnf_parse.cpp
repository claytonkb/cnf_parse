// cnf_parse.cpp
// g++ -std=c++11 -o cnf_parse cnf_parse.cpp numeric.cpp -lm

#include "cnf_parse.h"
#include <time.h>
#include <random>

void dev_prompt(void);
void dev_get_line(char *buffer, FILE *stream);
void dev_menu(void);

using namespace std;

int main(void){

    srand((unsigned)time(NULL));

    dev_prompt();

    _msg("Done");

}


// c ...
// ...
// p cnf var cls
// <int> ... <int> 0
// ...
// <int> ... <int> 0
clause_list *parse_DIMACS(char *dimacs_str){

    // XXX as a courtesy, should probably make a copy of dimacs_str so we can
    // treat the original as a const XXX

    clause_list *cl = (clause_list*)malloc(sizeof(clause_list));

    skip_comments(dimacs_str);
    read_format(dimacs_str, cl);

    cl->clauses   = (int*)malloc(sizeof(int)*cl->num_clauses);
    cl->variables = (int*)malloc(sizeof(int)*cl->num_variables);

    read_clauses(dimacs_str, cl);

    skip_ws(dimacs_str);
    // check we're at EOF (warn, don't error)

    return cl;

}


//
//
int read_format(char *dimacs_str, clause_list *cl){

    // p <format> <var> <cls>
    //      format=cnf
    //      vars=number of variables
    //      cls=number of clauses

    char *token = strtok(dimacs_str, " ");
    int var_int;

    if(!streq(token,"p")){
        _fatal("Expected 'p' line");
    }

    token = strtok(dimacs_str, " ");

    if(!streq(token,"cnf")){
        _fatal("Expected token 'cnf'");
    }

    token = strtok(dimacs_str, " ");
    var_int = atoi(token);
    // FIXME var_int check
    cl->num_variables = var_int;

    token = strtok(dimacs_str, " ");
    var_int = atoi(token);
    // FIXME var_int check
    cl->num_clauses = var_int;

}


//
//
int read_clauses(char *dimacs_str, clause_list *cl){

    int clause_ctr   = 0;
    int variable_ctr = 0;

    char *var_int_str;
    int   var_int;

    while(clause_ctr < cl->num_clauses){

        var_int=1;

        cl->clauses[clause_ctr] = variable_ctr;
        clause_ctr++;

        while(var_int){

            var_int_str = strtok(dimacs_str, " ");

            if(var_int_str == NULL){
                _fatal("Unexpected EOF");
            }

            var_int = atoi(var_int_str);
            // FIXME: Detect problems here

            // add var_int to cl->variables
            cl->variables[variable_ctr] = var_int;
            variable_ctr++;

        }
        //skip newline?

    }

}


int skip_comments(char *dimacs_str){

    int i=0;

    while(   dimacs_str[i  ] == 'c'
          && dimacs_str[i+1] == ' '){

        i+=2;

        while(dimacs_str[i] != 0xa && dimacs_str[i] != 0xd)
            i++;

        if(dimacs_str[i] == 0xa) // Windows CR+LF
            i++;

    }

//    if(dimacs_str[i] != 'p')
//        _fatal("expected 'p'");

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


//    cstring_vector* str_tokens = new vector<char*>;
//    char* cbuffer;
//    char *pch;
//
//    pch = strtok(cbuffer," \n");
//    bool comment_mode=false;
//
//    while(pch != NULL){
//
//        if(comment_mode){
//            if(streq(pch,"--"))
//                comment_mode = false;
//        }
//        else{
//            if(streq(pch,"--"))
//                comment_mode = true;
//            else
//                str_tokens->push_back(pch);
//        }
//
//        pch = strtok(NULL," \n");
//
//    }
//
//    return str_tokens;


//
//
void dev_prompt(void){

    char *cmd_code_str;
    int   cmd_code=0;

    char buffer[256];

    int i;

    _say("type 0 for menu");

    while(1){

        _prn("% ");

        dev_get_line(buffer, stdin);

        cmd_code_str = strtok(buffer, " ");
        if(cmd_code_str == NULL) continue;
        cmd_code = atoi(cmd_code_str);

        switch(cmd_code){
            case 0:
                dev_menu();
                break;

            case 1:
                _say("cmd_code==1");
                break;

            case 2:
                _say("exiting");
                return;

            case 3:
                _say("cmd_code==3");
                break;

            default:
                _say("unrecognized cmd_code");
                dev_menu();
                break;
        }

        for(i=0;i<256;i++){ buffer[i]=0; } // zero out the buffer

    }

}


//
//
void dev_get_line(char *buffer, FILE *stream){

    int c, i=0;

    while(1){ //FIXME unsafe, wrong
        c = fgetc(stream);
        if(c == EOF || c == '\n'){
            break;
        }
        buffer[i] = c;
        i++;
    }

    buffer[i] = '\0';

}


//
//
void dev_menu(void){

    _say( "\n0     .....    list command codes\n"
            "1     .....    dev one-off\n"
            "2     .....    exit\n"
            "3     .....    unimplemented\n");

}


// Clayton Bauman 2018

