// server2.cpp: 定义应用程序的入口点。
//
#include <iostream>
#include "apue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <syslog.h>
using namespace std;
constexpr auto QLEN = 10;

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

extern int intiserver(int, const struct sockaddr*, socklen_t, int);

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
			syslog(LOG_ERR, "ruptimed: unexpected return from exec: %s", strerror(errono));
		}
		else { /// parent
			close(clfd);
			waipid(pid, &status, 0);
		}
	}
}

int main(int argc, char** argv)
{
	struct addrinfo 
	return 0;
}
