#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char str[20] = {0};

    /* 校验传参 */
    if(argc != 2) 
    {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return -1;
	}

    sprintf(str, "hello %s!\n", argv[1]);
    while(1) 
    {
        printf("%s", str);
        sleep(1);
    }
    
    return 0;
}