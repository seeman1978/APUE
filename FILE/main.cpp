#include <iostream>
#include <cstdio>
#include "apue.h"

using namespace std;

/// the following is nonportable
#if defined(_IO_UNBUFFERED)
int is_unbuffered(std::FILE* fp){
    return fp->_flags & _IO_UNBUFFERED;
}

int is_linebuffered (FILE* fp){
    return fp->_flags & _IO_LINE_BUF;
}

int buffer_size(FILE* fp){
    return fp->_IO_buf_end - fp->_IO_buf_base;
}
#elif defined(__SNBF)
int is_unbuffered(std::FILE* fp){
    return fp->_flags & __SNBF;
}

int is_linebuffered (FILE* fp){
    return fp->_flags & __SLBF;
}

int buffer_size(FILE* fp){
    return fp->_bf._size;
}
#elif defined(_IONBF)
#ifdef LP64
#define _flag __pad[4]
#define ptr pad[1]
#define _base __pad[2]
#endif
int is_unbuffered(std::FILE* fp){
    return fp->_flags & _IONBF;
}

int is_linebuffered (FILE* fp){
    return fp->_flags & _IOLBF;
}

int buffer_size(FILE* fp){
#ifdef LP64
    return fp->_base - fp->_ptr;
#else
    return BUFSIZ;
#endif
}

#else
#error unknown stdio implementation!
#endif

void pr_stdio(const char* name, std::FILE* fp){
    std::cout << "stream = " << name << ", ";
    if (is_unbuffered(fp)){
        std::cout << " unbuffered";
    }
    else if(is_linebuffered(fp)){
        std::cout << "line buffered";
    }
    else{
        std::cout << "fully buffered";
    }
    cout << ", buffer size = " << buffer_size(fp) << '\n';
}



int main() {
    std::FILE *fp;
    fputs("enter any character \n ", stdout);
    if (getchar() == EOF){
        err_sys("getchar error");
    }
    fputs("one line to standard error\n " , stderr);
    pr_stdio("stdin", stdin);
    pr_stdio("stdout", stdout);
    pr_stdio("stderr", stderr);
    if ((fp = fopen("/etc/passwd", "r")) == nullptr){
        err_sys("fopen error");
    }
    if (getc(fp) == EOF){
        err_sys("getc error");
    }
    pr_stdio("/etc/passwd", fp);

    return 0;
}
