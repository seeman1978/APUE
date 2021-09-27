#include <iostream>
#include <csetjmp>
#include "apue.h"

const auto TOK_ADD = 5;
jmp_buf jmpbuffer;
char* tok_ptr;

int get_token(){

}

void cmd_add(){
    int token;
    token = get_token();
    if (token < 0){
        longjmp(jmpbuffer, 1);
    }
}

void do_line(char* ptr){
    int cmd;
    tok_ptr = ptr;
    while ((cmd = get_token()) > 0){
        switch (cmd) {
            case TOK_ADD:
                cmd_add();
                break;
        }
    }
}

int main() {
    char line[MAXLINE];
    if (setjmp(jmpbuffer) != 0){
        std::cout << "error";
    }
    while (fgets(line, MAXLINE, stdin) != nullptr){
        do_line(line);
    }
    return 0;
}
