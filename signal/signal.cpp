// signal.cpp: 定义应用程序的入口点。
//

#include <iostream>
#include "apue.h"
using namespace std;

void sig_usr(int signo)
{
	if (signo == SIGUSR1)
	{
		std::cout << "received SIGUSER1\n";
	}
	else if (signo == SIGUSR2)
	{
		std::cout << "received SIGUSER2\n";
	}
	else
	{
		err_dump("received signal %d\n", signo);
	}
}

int main()
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
	{
		err_sys("can't catch SIGUSR1");
	}
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
	{
		err_sys("can't catch SIGUSR2");
	}
	for(;;)
	{
		pause();
	}
    return 0;
}
