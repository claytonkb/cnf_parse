// cnf_parse.cpp
// g++ -std=c++11 -o cnf_parse cnf_parse.cpp numeric.cpp -lm

#include "cnf_parse.h"
#include <time.h>
#include <random>

void dev_prompt(void);
void dev_get_line(char *buffer, FILE *stream);
void dev_menu(void);
char *slurp_file(char *filename);
FILE *open_file(char *filename, const char *attr);
int file_size(FILE *file);

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


//
//
void dev_prompt(void){

    char *cmd_code_str;
    int   cmd_code=0;

    char buffer[256];

    int i;

    char *cnf_file;
    clause_list *cl;

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
                cmd_code_str = strtok(NULL, " ");
                if(cmd_code_str == NULL){ _say("not enough arguments"); continue; }
                cnf_file = slurp_file(cmd_code_str);
//                printf("%s\n\n", cnf_file);
                break;

            case 4:
                cl = parse_DIMACS(cnf_file);
                break;

            case 5:
                _mem(cl->clauses, cl->num_clauses);
                break;

            case 6:
                _mem(cl->variables, cl->num_assignments);
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
            "3     .....    open cnf_file\n"
            "4     .....    parse DIMACS\n");

}


//
//
char *slurp_file(char *filename){

    FILE *f = open_file((char*)filename, "r");
    int size = file_size(f);

    char *file_buffer = (char*)malloc(size+1);
    size_t dummy = fread((char*)file_buffer, 1, size, f);

    fclose(f);

    return file_buffer;

}


//
//
FILE *open_file(char *filename, const char *attr){

    FILE* file;

    file = fopen((char*)filename, attr);

    if(file==NULL)
        _fatal((char*)filename);

    return file;

}


//
//
int file_size(FILE *file){

    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    rewind(file);

    return size;

}



// Clayton Bauman 2018

