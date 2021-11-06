#include <iostream>
#include "opend.h"
#include "apue.h"
char errmsg[MAXLINE];
int oflag;
char *pathname;
int main() {
    int nread;
    char buf[MAXLINE];
    for (;;){   ///read arg buffer from client, process request
        if (nread = read(STDIN_FILENO, buf, MAXLINE); nread < 0){
            err_sys("read error no stream pipe");
        }
        else if (nread == 0){
            break;  ///client has closed the stream pipe
        }
        handle_request(buf, nread, STDOUT_FILENO);
    }
    return 0;
}
