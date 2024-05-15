#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int write_file(const char *file_path, const char *data);
int read_file(const char *file_path, char *data, size_t length);

int main(int argc, char *argv[])
{
    char gpio_path[64] = {0};
    char file_path[64] = {0};
    char buffer[8] = {0};

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
       
    /* 配置为非中断方式 */
    sprintf(file_path, "%s/%s", gpio_path, "edge");
    if(write_file(file_path, "none"))
        return -1;
        
    /* 读取 GPIO 电平状态 */
    sprintf(file_path, "%s/%s", gpio_path, "value");
    if(read_file(file_path, buffer, 1))
        return -1;
    printf("value: %s\n", buffer);

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

int read_file(const char *file_path, char *data, size_t length)
{
    int fd = open(file_path, O_RDONLY);
    if(fd < 0) 
    {
        perror("open error");
        return -1;
    }

    int ret = read(fd, data, length);
    if(ret != length) 
    {
        perror("read error");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}