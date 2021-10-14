#include <iostream>
#include <sys/syslog.h>
#include <cstring>
#include <csignal>
#include <sys/resource.h>
#include <fcntl.h>
#include "apue.h"

extern int lockfile(int);
extern int already_running(void);

void daemonize(const char* cmd){
    int fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;
    ///clear file creation mask
    umask(0);

    ///get maximum number of file descriptors
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0){
        err_quit("%s:: can't get file limit", cmd);
    }

    /// become a seesion leader to lose controlling TTY
    if (pid = fork(); pid < 0){
        err_quit("%s:can't fork", cmd);
    }
    else if (pid != 0){///parent
        exit(0);
    }
    setsid();

    ///ensure future opens won't allocate controlling TTYs
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, nullptr) < 0){
        err_quit("%s:can't ignore SIGHUP", cmd);
    }
    if (pid = fork(); pid < 0){
        err_quit("%s:can't fork", cmd);
    }
    else if (pid != 0){///parent
        exit(0);
    }

    ///change current working directory to the root so we don't prevent file systems from being unmounted
    if (chdir("/") < 0){
        err_quit("%s:can't change directory to /", cmd);
    }

    ///close all open file descriptors
    if (rl.rlim_max = RLIM_INFINITY){
        rl.rlim_max = 1024;
    }
    for (int i = 0; i < rl.rlim_max; ++i) {
        close(i);
    }

    ///attach file descriptor 0, 1, 2 to /dev/null
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    /// initialize log file
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2){
        syslog(LOG_ERR, "unexpected file descriptor %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}

static void reread(){

}

void sigterm(int signo){
    syslog(LOG_INFO, "got SIGTERM ; exiting");
    exit(0);
}

void sighup(int signo){
    syslog(LOG_INFO, "Re-reading configure file");
    reread();
}

int main(int argc, char** argv) {
    char* cmd;
    struct sigaction sa;

    if (cmd = strrchr(argv[0], '/'); cmd == nullptr){
        cmd = argv[0];
    }
    else{
        cmd++;
    }
    /// become a daemon
    daemonize(cmd);

    /// make sure only one copy of the daemon is running
    if (already_running()){
        syslog(LOG_ERR, "daemon already running");
        exit(1);
    }

    ///restore SIGHUP default and block all signals
    sa.sa_handler = sigterm;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGHUP);
    sa.sa_flags = 0;
    if (sigaction(SIGTERM, &sa, nullptr) < 0){
        syslog(LOG_ERR, "can't catch SIGTERM:%s", strerror(errno));
        exit(1);
    }

    sa.sa_handler = sighup;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGTERM);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, nullptr) < 0){
        syslog(LOG_ERR, "can't catch SIGHUP:%s", strerror(errno));
        exit(1);
    }

    ///proceed with the rest of the daemon

    return 0;
}
