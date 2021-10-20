#include <iostream>
#include "apue.h"
constexpr auto PAGER = "${PAGER:-more}";
int main(int argc, char** argv) {
    char line[MAXLINE];
    FILE* fpin, *fpout;
    if (argc != 2){
        err_quit("usage a.out <pathname>");
    }
    if (fpin = fopen(argv[1], "r"); fpin == nullptr){
        err_sys("can't open %s", argv[1]);
    }
    if (fpout = popen(PAGER, "w"); fpout == nullptr){
        err_sys("popen error");
    }
    ///copy argv[1] to pager
    while (fgets(line, MAXLINE, fpin) != nullptr){
        if (fputs(line, fpout) == EOF){
            err_sys("fputs error to pipe");
        }
    }
    if (ferror(fpin)){
        err_sys("fgets error");
    }
    if (pclose(fpout) == -1){
        err_sys("pclose error");
    }
    return 0;
}
