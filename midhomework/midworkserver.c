//
// Created by zhoucj on 2019/11/2.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){


	char buf[] = "00001234";
	printf("aaa %d", atoi(&buf));

	int as=1234;
	char bufs[8]={0};
	memset(bufs,'0',4);
	sprintf(&bufs, "%s%d", &bufs,as);
	printf(&bufs);
    return 0;
}
