// fgets.cpp: 定义应用程序的入口点。
//

#include "fgets.h"
#include "apue.h"
using namespace std;

int main()
{
	char buf[MAXLINE];
	while (fgets(buf, MAXLINE, stdin) != nullptr)
	{
		if (fputs(buf, stdout) == EOF)
		{
			err_sys("output error");
		}
	}
	if (ferror(stdin))
	{
		err_sys("input error");
	}
	return 0;
}
