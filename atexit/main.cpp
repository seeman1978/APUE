#include <iostream>
#include "apue.h"

void my_exit1(){
    std::cout << "first exit handler\n";
}

void my_exit2(){
    std::cout << "second exit handler\n";
}

int main() {
    if (atexit(my_exit2) != 0){
        err_sys("can't register my_exit2");
    }
    if (atexit(my_exit1) != 0){
        err_sys("can't register my_exit1");
    }
    if (atexit(my_exit1) != 0){
        err_sys("can't register my_exit1");
    }
    std::cout << "main is done\n";
    return 0;
}

