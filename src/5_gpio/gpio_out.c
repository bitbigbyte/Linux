#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int write_file(const char *file_path, const char *data);

int main(int argc, char *argv[])
{
    char gpio_path[64] = {0};
    char file_path[64] = {0};

    /* 校验传参 */
    if(argc != 3) 
    {
        fprintf(stderr, "Usage: %s <gpio_pin> <gpio_pin_state>\n", argv[0]);
        return -1;
    }

    /* 导出 gpio */
    sprintf(gpio_path, "/sys/class/gpio/gpio%s", argv[1]);
    if(access(gpio_path, F_OK))
        if(write_file("/sys/class/gpio/export", argv[1]))
            return -1;
    
    /* 配置为输出模式 */
    sprintf(file_path, "%s/%s", gpio_path, "direction");
    if(write_file(file_path, "out"))
        return -1;
      
    /* 极性设置 */
    sprintf(file_path, "%s/%s", gpio_path, "active_low");
    if(write_file(file_path, "0"))
        return -1;
       
    /* 配置为非中断方式 */
    sprintf(file_path, "%s/%s", gpio_path, "edge");
    if(write_file(file_path, "none"))
        return -1;
        
    /* 设置 GPIO 电平状态 */
    sprintf(file_path, "%s/%s", gpio_path, "value");
    if(write_file(file_path, argv[2]))
        return -1;

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
