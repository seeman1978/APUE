#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include "apue.h"

constexpr auto BUFLEN = 128;
extern int connect_retry(int, int, int, const struct socketaddr*, socklen_t);
void print_uptime(int sockfd){
    ssize_t n;
    char buf[BUFLEN];
    while ((n=recv(sockfd, buf, BUFLEN, 0)) > 0){
        write(STDOUT_FILENO, buf, n);
    }
    if (n < 0){
        err_sys("recv error");
    }
}

int main(int argc, char** argv) {
    struct addrinfo *ailist, *aip;
    struct addrinfo hint{};
    int sockfd, err;
    if (argc != 2){
        err_quit("usage: ruptime hostname");
    }
    memset(&hint, 0, sizeof hint);
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = nullptr;
    hint.ai_addr = nullptr;
    hint.ai_next = nullptr;
    if (err = getaddrinfo(argv[1], "ruptime", &hint, &ailist); err != 0){
        err_quit("getaddrinfo error : %s", gai_strerror(err));
    }
    for (aip = ailist; aip != nullptr; aip = aip->ai_next){
        if (sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0,
                                   reinterpret_cast<const struct socketaddr *>(aip->ai_addr), aip->ai_addrlen); sockfd < 0){
            err = errno;
        }
        else{
            print_uptime(sockfd);
            exit(0);
        }
    }
    err_exit(err, "can't connect to %s", argv[1]);
}
