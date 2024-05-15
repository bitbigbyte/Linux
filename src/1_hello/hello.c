#include <stdio.h>

int main(int argc, char *argv[]) 
{
	/* 校验传参 */
    if(argc != 2) 
	{
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return -1;
	}

	printf("hello %s!\n", argv[1]);
	
	return 0;
}