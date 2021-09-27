#include <iostream>
#include <csetjmp>

#include "apue.h"
jmp_buf jmpbuffer;
int globval;

void f2(){
    longjmp(jmpbuffer, 1);
}

void f1(int i, int j, int k, int l){
    std::cout << "in f1():\n";
    printf("golval = %d, autoval=%d, regival=%d, volaval=%d, statval=%d\n",
           globval, i, j, k, l);
    f2();
}

int main() {
    int autoval;
    register int regival;
    volatile int volaval;
    static int statval;

    globval = 1;
    autoval = 2;
    regival = 3;
    volaval = 4;
    statval = 5;
    if (setjmp(jmpbuffer) != 0){
        std::cout << "after longjmp:\n";
        printf("globval=%d, autoval=%d, regival=%d, volaval = %d, statval = %d",
               globval, autoval, regival, volaval,statval);
        exit(0);
    }
    globval = 95;
    autoval = 96;
    regival = 97;
    volaval = 98;
    statval = 99;
    f1(autoval, regival, volaval, statval);
    return 0;
}
