#include <iostream>
#include "apue.h"
#include "opend.h"

int debug, cflag, client_size, log_to_stderr;
char errmsg[MAXLINE];
char *pathname;
Client *client = nullptr;
int main(int argc, char** argv) {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
