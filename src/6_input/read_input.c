#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int fd;
    struct input_event input_event = {0};

    /* 校验传参 */
    if(argc != 2) 
    {
        fprintf(stderr, "Usage: %s <input_dev>\n", argv[0]);
        return -1;
    }

    /* 打开文件 */
    if(0 > (fd = open(argv[1], O_RDONLY))) 
    {
        perror("open error");
        return -1;
    }

    /* 循环读取数据 */
    while(1)
    {
        if(sizeof(struct input_event) != read(fd, &input_event, sizeof(struct input_event))) 
        {
            perror("read error");
            return -1;
        }
        printf("time:%f, type:%d, code:%d, value:%d\n", 
                input_event.time.tv_sec+input_event.time.tv_usec*pow(10,-6), 
                input_event.type, 
                input_event.code, 
                input_event.value);
    }

    return 0;
}