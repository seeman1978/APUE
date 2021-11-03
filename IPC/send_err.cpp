/**
* @file
* @brief
* @details
* @author		qiangwang
* @date		    2021/11/3
* @par Copyright(c): 	2021 mega. All rights reserved.
*/

#include "apue.h"

/// used when we had planned to send an fd using send_fd(),
/// but encountered an error instead. We send the error back
/// using the send_fd()/recv_fd() prototol
int send_err(int fd, int errcode, const char* msg){
    size_t n;
    if (n = strlen(msg); n > 0){
        if (write(fd, msg, n) != n){    /// sned the error message
            return -1;
        }
    }
    if (errcode >= 0){
        errcode = -1;   /// must be negative
    }
    if (send_fd(fd, errcode) < 0){
        return -1;
    }
    return 0;
}
