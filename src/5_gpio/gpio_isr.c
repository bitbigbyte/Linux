#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>

int write_file(const char *file_path, const char *data);

int main(int argc, char *argv[])
{
    char gpio_path[64] = {0};
    char file_path[64] = {0};
    char buffer[8] = {0};
    struct pollfd pfd;

    /* 校验传参 */
    if(argc != 2) 
    {
        fprintf(stderr, "Usage: %s <gpio_pin>\n", argv[0]);
        return -1;
    }

    /* 导出 gpio */
    sprintf(gpio_path, "/sys/class/gpio/gpio%s", argv[1]);
    if(access(gpio_path, F_OK))
        if(write_file("/sys/class/gpio/export", argv[1]))
            return -1;

    /* 配置为输入模式 */
    sprintf(file_path, "%s/%s", gpio_path, "direction");
    if(write_file(file_path, "in"))
        return -1;
      
    /* 极性设置 */
    sprintf(file_path, "%s/%s", gpio_path, "active_low");
    if(write_file(file_path, "0"))
        return -1;
       
    /* 配置为双边沿中断方式 */
    sprintf(file_path, "%s/%s", gpio_path, "edge");
    if(write_file(file_path, "both"))
        return -1;
        
    /* 读取 GPIO 电平状态 */
    sprintf(file_path, "%s/%s", gpio_path, "value");
    pfd.fd = open(file_path, O_RDONLY);
    if(pfd.fd < 0) 
    {
        perror("open error");
        return -1;
    }
    pfd.events = POLLPRI; //只关心高优先级数据可读
    if(read(pfd.fd, buffer, 1) != 1) //先读取一次清除状态
    { 
        perror("read error");
        close(pfd.fd);
        exit(-1);
    }
    while(1)
    {
        if(poll(&pfd, 1, -1) < 0) //阻塞等待
        { 
            perror("poll error");
            close(pfd.fd);
            exit(-1);
        }

        if(pfd.revents & POLLPRI) 
        {
            if(lseek(pfd.fd, 0, SEEK_SET))
            {
                perror("lseek error");
                close(pfd.fd);
                exit(-1);
            }
            if(1 != read(pfd.fd, buffer, 1)) 
            {
                perror("read error");
                close(pfd.fd);
                exit(-1);
            }
            printf("GPIO 中断触发<value=%s>\n", buffer);
        }
    }

    /* 退出程序 */
    return 0;
}

int write_file(const char *file_path, const char *data)
{
    int fd = open(file_path, O_WRONLY);
    if(fd < 0) 
    {
        perror("open error");
        return -1;
    }

    int ret = write(fd, data, strlen(data));
    if(ret != strlen(data)) 
    {
        perror("write error");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}