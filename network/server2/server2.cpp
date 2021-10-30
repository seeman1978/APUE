// server2.cpp: 定义应用程序的入口点。
//
#include <iostream>
#include "apue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <syslog.h>
#include <netdb.h>
using namespace std;
constexpr auto QLEN = 10;

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

extern int initserver(int, const struct sockaddr*, socklen_t, int);

void serve(int sockfd) {
	int clfd, status;
	pid_t pid;
	set_cloexec(sockfd);
	for (;;) {
		if (clfd = accept(sockfd, nullptr, nullptr); clfd < 0) {
			syslog(LOG_ERR, "ruptimed: accept error : %s", strerror(errno));
			exit(1);
		}
		if (pid = fork(); pid < 0) {
			syslog(LOG_ERR, "ruptimed: fork error: %s", strerror(errno));
			exit(1);
		}
		else if (pid == 0) {	/// child
			if (dup2(clfd, STDOUT_FILENO) != STDOUT_FILENO || 
				dup2(clfd, STDERR_FILENO) != STDERR_FILENO) {
				syslog(LOG_ERR, "ruptimed: unexpected error");
				exit(1);
			}
			close(clfd);
			execl("/usr/bin/uptime", "uptime", (char*)nullptr);
			syslog(LOG_ERR, "ruptimed: unexpected return from exec: %s", strerror(errno));
		}
		else { /// parent
			close(clfd);
			waitpid(pid, &status, 0);
		}
	}
}

int main(int argc, char** argv)
{
	struct addrinfo* ailist, * aip;
	struct addrinfo hint;
	int sockfd, err;
    size_t n;
	char* host;
	if (argc != 1) {
		err_quit("usage: ruptimed");
	}
	if (n = sysconf(_SC_HOST_NAME_MAX); n < 0) {
		n = HOST_NAME_MAX;
	}
	if (host = static_cast<char *>(malloc(n)); host == nullptr) {
		err_sys("malloc error");
	}
	if (gethostname(host, n) < 0) {
		err_sys("gethostname error");
	}
	daemonize("ruptimed");
	memset(&hint, 0, sizeof hint);
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = nullptr;
	hint.ai_addr = nullptr;
	hint.ai_next = nullptr;
	if (err = getaddrinfo(host, "ruptime", &hint, &ailist); err != 0) {
		syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s", gai_strerror(err));
		exit(1);
	}
	for (aip = ailist; aip != nullptr; aip = aip->ai_next) {
		if (sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN); sockfd >= 0) {
			serve(sockfd);
			exit(0);
		}
	}
	return 0;
}
