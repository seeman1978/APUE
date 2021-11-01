/**
* @file
* @brief
* @details
* @author		qiangwang
* @date		    2021/11/1
* @par Copyright(c): 	2021 mega. All rights reserved.
*/

#include "apue.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <errno.h>

constexpr auto STALE = 30;  /// client's name can't be older than this (seconds)

/**
 * Wait for a client connection to arrive, and accept it.
 * We also abtain the client's user ID from the pathname that it must bind before calling us.
 * Returns new fd if all OK, <0 on error.
 * */

int serv_accept(int listenfd, uid_t *uidptr){
    int clifd, err, rval;
    socklen_t len;
    time_t staletime;
    struct sockaddr_un un;
    struct stat statbuf;
    char *name;
    ///allocate enough space for longest name plus terminating null
    if (name=malloc(sizeof(un.sun_path)+1); name == nullptr){
        return -1;
    }
    len = sizeof un;
    if (clifd = accept(listenfd, (struct sockaddr*)&un, &len); clifd < 0){
        free(name);
        return -2;  /// often errno=EINTR, if signal caught
    }
    ///obtain the client's uid from its calling address
    len -= offsetof(struct sockaddr_un, un.sun_path); /// len of pathname
    memcpy(name, un.sun_path, len);
    name[len] = 0;///null terminate
    if (stat(name, &statbuf) < 0){
        rval = -3;
        goto errout;
    }
#ifdef S_ISSOCK///NOT DEFINED FOR SVR4
    if (S_ISSOCK(statbuf.st_mode) == 0){
        rval = -4;
        goto errout;
    }
#endif
    if ((statbuf.st_mode & (S_IRWXG|S_IRWXO)) || (statbuf.st_mode & S_IRWXU)!=S_IRWXU){
        rval = -5;      //is not rwx
        goto errout;
    }
    staletime = time(nullptr) - STALE;
    if (statbuf.st_atim < staletime || statbuf.st_ctim < staletime || statbuf.st_mtim < staletime){
        rval = -6;///i-node is too old
        goto errout;
    }
    if (uidprt != nullptr){
        *uidptr = statbuf.st_uid;   ///return uid of caller
    }
    unlink(name);
    free(name);
    return clifd;
errout:
    err = errno;
    close(clifd);
    free(name);
    errno = err;
    return rval;
}