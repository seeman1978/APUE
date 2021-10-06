// signal2.cpp: 定义应用程序的入口点。
//

#include  "apue.h"
#include <iostream>
#include <pwd.h>

using namespace std;

void my_alarm(int signo)
{
	struct passwd* rootptr;
	cout << "in signal handler \n";
	if (rootptr = getpwnam("root"); rootptr == nullptr)
	{
		err_sys("getpwname(root) error");
	}
	alarm(1);
}

int main()
{
	struct passwd* ptr;
	signal(SIGALRM, my_alarm);
	alarm(1);
	for (;;) {
		if (ptr = getpwnam("wq"); ptr == nullptr) {
			err_sys("getpwnam error");
		}
		if (strcmp(ptr->pw_name, "wq") != 0) {
			cout << "return value corrupted!, pw_name =" << ptr->pw_name << "\n";
		}
	}
	
	return 0;
}
