
/**
* @file
* @brief
* @details
* @author		qiangwang
* @date		    2021/11/6
* @par Copyright(c): 	2021 mega. All rights reserved.
*/

#include "open.h"
#include "apue.h"
#include <fcntl.h>
constexpr auto BUFFSIZE = 8192;

int main(int argc, char** argv){
    int fd;
    size_t n;
    char buf[BUFFSIZE];
    char line[MAXLINE];
    ///read file name to cat from stdin
    while(fgets(line, MAXLINE, stdin) != nullptr){
        if (line[strlen(line) - 1] == '\n'){
            line[strlen(line) - 1] = 0; /// replace newline with null
        }
        ///open the file
        if (fd = csopen(line, O_RDONLY); fd < 0){
            continue;   /// csopen prints error from server
        }
        ///and cat to stdout
        while ((n= read(fd, buf, BUFFSIZE)) > 0){
            if (write(STDOUT_FILENO, buf, n) != n){
                err_sys("write error");
            }
        }
        if (n < 0){
            err_sys("read error");
        }
        close(fd);
    }
    exit(0);
}
