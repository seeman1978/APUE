// getc.cpp: 定义应用程序的入口点。
//

#include "getc.h"
#include "apue.h"

using namespace std;

int main()
{
	int c;
	while((c=getc(stdin)) != EOF){
		if (putc(c, stdout) == EOF)
		{
			err_sys("output error");
		}
	}
	return 0;
}
