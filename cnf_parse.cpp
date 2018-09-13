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

