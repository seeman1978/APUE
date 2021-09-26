#include <iostream>
#include "apue.h"
int main() {
    char name[L_tmpnam]{"./indi_XXXXXX"}, line[MAXLINE];
    FILE* fp;
    int tmpfd = mkstemp(name);
    if (tmpfd == -1){
        err_sys("mkstemp failed");
    }
    unlink(name);

    std::cout << name << '\n';
    fp = tmpfile();
    if (fp == nullptr){
        err_sys("tmpfile error");
    }
    fputs("one line of output\n", fp);
    rewind(fp);
    if (fgets(line, sizeof(line), fp) == nullptr){
        err_sys("fgets error");
    }
    fputs(line, stdout);
    return 0;
}
