#include <iostream>

int main() {
    time_t t;
    struct tm *tmp;
    char buf1[16], buf2[64];
    time(&t);
    tmp = localtime(&t);
    if (strftime(buf1, 16, "time and date: %r, %a %b %d, %Y", tmp) == 0){
        std::cout << "buffer length 16 is too small\n";
    }
    else{
        std::cout << buf1 << '\n';
    }
    if (strftime(buf2, 64, "time and date: %r, %a %b %d, %Y", tmp) == 0){
        std::cout << "buffer length 64 is too small\n";
    }
    else{
        std::cout << buf2 << '\n';
    }
    return 0;
}
