#include <iostream>
#include <sys/socket.h>
#include <syslog.h>
#include <netdb.h>
#include "apue.h"

constexpr auto BUFLEN = 128;
constexpr auto QLEN = 10;
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

extern int initserver(int type, const struct sockaddr *addr, socklen_t alen,
                      int qlen);

void serve(int sockfd){
    int clfd;
    FILE *fp;
    char buf[BUFLEN];
    set_cloexec(sockfd);
    for (;;){
        if (clfd = accept(sockfd, nullptr, nullptr); clfd < 0){
            syslog(LOG_ERR, "RUNTIMED: accept error:%s", strerror(errno));
            exit(1);
        }
        set_cloexec(clfd);
        /// popen()会调用fork()产生子进程，然后从子进程中调用/bin/sh -c 来执行参数command 的指令。
        if (fp = popen("/bin/bin/uptime", "r"); fp == nullptr){
            sprintf(buf, "error:%s\n", strerror(errno));
            send(clfd, buf, strlen(buf), 0);
        }
        else{
            while (fgets(buf, BUFLEN, fp) != nullptr){
                send(clfd, buf, strlen(buf), 0);
            }
            pclose(fp);
        }
        close(clfd);
    }
}

int main(int argc, char** argv) {
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, err, n;
    char *host;
    if (argc != 1){
        err_quit("usage: ruptimed");
    }
    if (n = sysconf(_SC_HOST_NAME_MAX); n < 0){
        n = HOST_NAME_MAX;
    }
    if (host = static_cast<char *>(malloc(n)); host == nullptr){
        err_sys("malloc error");
    }
    if (gethostname(host, n) < 0){
        err_sys("gethostname error");
    }
    daemonize("ruptimed");
    memset(&hint, 0, sizeof hint);
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = nullptr;
    hint.ai_addr = nullptr;
    hint.ai_next = nullptr;
    if (err = getaddrinfo(host, "ruptime", &hint, &ailist); err != 0){
        syslog(LOG_ERR, "ruptimed:getaddrinfo error:%s", gai_strerror(err));
        exit(1);
    }
    for (aip = ailist; aip != nullptr; aip = aip->ai_next){
        if (sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN); sockfd >= 0){
            serve(sockfd);
            exit(0);
        }
    }
    return 1;
}
