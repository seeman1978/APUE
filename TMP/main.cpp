#include <iostream>
#include "apue.h"
int main() {
    char name[L_tmpnam]{"./indi_XXXXXX"}, line[MAXLINE];
    FILE *fp1, *fp2;
    int tmpfd = mkstemp(name);
    if (tmpfd == -1){
        err_sys("mkstemp failed");
    }
    //unlink(name);
    fp1 = fdopen(tmpfd, "a+");
    fputs("one line of output\n", fp1);
    rewind(fp1);
    if (fgets(line, sizeof(line), fp1) == nullptr){
        err_sys("fgets error");
    }
    fputs(line, stdout);

    std::cout << name << '\n';
    fp2 = tmpfile(); /// auto unlink.
    if (fp2 == nullptr){
        err_sys("tmpfile error");
    }
    fputs("one line of output\n", fp2);
    rewind(fp2);
    if (fgets(line, sizeof(line), fp2) == nullptr){
        err_sys("fgets error");
    }
    fputs(line, stdout);
    return 0;
}
