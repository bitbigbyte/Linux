#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define DEBUG
#ifdef DEBUG
#define DBG(...) fprintf(stderr, " DBG(%s, %s(), %d): ", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif

int main(int argc, char *argv[])
{
	int fd1,fd2;

	/* 校验传参 */
    if(argc != 2) 
	{
		fprintf(stderr, "Usage: %s <on|off|timer|heartbeat>\n", argv[0]);
        return -1;
	}

	/* 打开文件 */
	fd1 = open("/sys/class/leds/sys-led/trigger", O_RDWR);
	if(fd1 < 0) 
	{
		perror("open error");
		return -1;
	}
	fd2 = open("/sys/class/leds/sys-led/brightness", O_RDWR);
	if(fd2 < 0) 
	{
		perror("open error");
		close(fd1);
		return -1;
	}

	/* 控制LED */
    if(!strcmp(argv[1], "on")) 
	{
        DBG("LED常亮!\n");
		if(write(fd1, "none", 4) != 4)
			goto error;
		if(write(fd2, "1", 1) != 1) 
			goto error;
    } 
	else if(!strcmp(argv[1], "off")) 
	{
		DBG("LED熄灭!\n");
		if(write(fd1, "none", 4) != 4)
			goto error;
		if(write(fd2, "0", 1) != 1)
			goto error;
    } 
	else if(!strcmp(argv[1], "timer")) 
	{
        DBG("LED定时器模式!\n");
		if(write(fd1, "timer", 5) != 5) 
			goto error;
    } 
	else if(!strcmp(argv[1], "heartbeat")) 
	{
		DBG("LED呼吸灯\n");
		if(write(fd1, "heartbeat", 9) != 9)
			goto error;
	}

	/* 关闭文件 */
	close(fd1);
	close(fd2);
    return 0;

error:
	perror("write error");
	close(fd1);
	close(fd2);
	return -1;
}