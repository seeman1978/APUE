#include <iostream>
#include "apue.h"
int main() {
    int int1, int2;
    char line[MAXLINE];

    ///if there are not these four lines code, the program will be deadlock
    if (setvbuf(stdin, nullptr, _IOLBF, 0) != 0){
        err_sys("setvbuf error");
    }
    if (setvbuf(stdout, nullptr, _IOLBF, 0) != 0){
        err_sys("setvbuf error");
    }

    while (fgets(line, MAXLINE, stdin) != nullptr){
        if (sscanf(line, "%d%d", &int1, &int2) == 2){
            if (printf(":%d\n", int1+int2) == EOF){
                err_sys("printf error");
            }
        }
        else{
            if (printf("invalid args\n") == EOF){
                err_sys("printf error");
            }
        }
    }
    return 0;
}
