#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>

#define argb8888_to_rgb565(color)   ({ \
            unsigned int temp = (color); \
            ((temp & 0xF80000UL) >> 8) | \
            ((temp & 0xFC00UL) >> 5) | \
            ((temp & 0xF8UL) >> 3); \
            })
#define DEBUG
#ifdef DEBUG
#define DBG(...) fprintf(stderr, " DBG(%s, %s(), %d): ", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif            
#define FB_DEV "/dev/fb0"

/* BMP文件头数据结构 */
typedef struct {
    unsigned char type[2];      //文件类型
    unsigned int size;          //文件大小
    unsigned short reserved1;   //保留字段1
    unsigned short reserved2;   //保留字段2
    unsigned int offset;        //到位图数据的偏移量
} __attribute__ ((packed)) bmp_file_header;

/* 位图信息头数据结构 */
typedef struct {
    unsigned int size;              //位图信息头大小
    int width;                      //图像宽度
    int height;                     //图像高度
    unsigned short planes;          //位面数
    unsigned short bpp;             //像素深度 
    unsigned int compression;       //压缩方式
    unsigned int image_size;        //图像大小
    int x_pels_per_meter;           //像素/米
    int y_pels_per_meter;           //像素/米 
    unsigned int clr_used;
    unsigned int clr_omportant;
} __attribute__ ((packed)) bmp_info_header;

unsigned int width;        //LCD X分辨率
unsigned int height;       //LCD Y分辨率
unsigned int bpp;          //LCD像素深度
unsigned short *fb_base;   //映射后的显存基地址
unsigned int fb_size;	   //映射后的显存大小
int show_bmp(const char *path);

int main(int argc, char **argv)
{
    int fd;
    int ret;
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;

    /* 传参校验 */
    if(argc != 2) 
    {
        fprintf(stderr, "Usage: %s <bmp_file>\n", argv[0]);
        return -1;
    }

    /* 打开framebuffer设备 */
    fd = open(FB_DEV, O_RDWR);
    if(fd < 0) 
    {
        perror("open LCD error");
        return -1;
    }

    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);
    fb_size = fb_fix.line_length * fb_var.yres;
    width = fb_var.xres;
    height = fb_var.yres;
    bpp = fb_var.bits_per_pixel;
    DBG("LCD分辨率: %d*%d\n", width, height);
	DBG("LCD像素深度: %d\n", bpp);
    if(bpp != 16)
    {
        fprintf(stderr, "This program only supports LCD with a bpp of 16!\n");
        close(fd);
        return -1;
    }

    /* 将显示缓冲区映射到进程地址空间 */
    fb_base = mmap(NULL, fb_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if((void *)fb_base == MAP_FAILED) 
    {
        perror("mmap LCD error");
        close(fd);
        return -1;
    }

    /* 显示BMP图片 */
    memset(fb_base, 0xff, fb_size); //刷白屏幕
    ret = show_bmp(argv[1]);
    if(ret < 0)
    {
        fprintf(stderr, "show BMP error!\n");
        munmap(fb_base, fb_size);   
        close(fd);
        return -1;                  
    }

    /* 退出 */
    munmap(fb_base, fb_size);   //取消映射
    close(fd);                  //关闭文件
    return 0;                   //退出进程
}

int show_bmp(const char *path)
{
    int fd;
    int ret;
    bmp_file_header file_header;
    bmp_info_header info_header;
    unsigned int min_width;
    unsigned int min_height;
	unsigned char *line_buffer; //行缓冲区:用于存储从bmp文件取出的一行数据
    int direction_flag;         //位图方向标记，0表示倒向，1表示正向

    /* 打开文件 */
    fd = open(path, O_RDONLY);
    if(fd < 0) 
    {
        perror("open BMP error");
        return -1;
    }

    /* 读取BMP文件头 */
    ret = read(fd, &file_header, sizeof(bmp_file_header));
    if(ret != sizeof(bmp_file_header))
    {
        perror("read BMP file_header error");
        close(fd);
        return -1;
    }
    if(memcmp(file_header.type, "BM", 2) != 0) 
    {
        fprintf(stderr, "It's not a BMP file!\n");
        close(fd);
        return -1;
    }

    /* 读取位图信息头 */
    ret = read(fd, &info_header, sizeof(bmp_info_header));
    if(ret != sizeof(bmp_info_header)) 
    {
        perror("read BMP info_header error");
        close(fd);
        return -1;
    }

    /* 判断位图方向 */
    direction_flag = info_header.height > 0 ? 0 : 1; //如果info_header.height大于0，则为倒向位图；反之，info_header.height小于0，则为正向位图
    info_header.height = abs(info_header.height);    //取绝对值

    /* 打印信息 */
    DBG("BMP图像分辨率: %d*%d\n", info_header.width, info_header.height);
    DBG("BMP图像像素深度: %d\n", info_header.bpp);

    /* 将文件读写位置移动到图像数据开始处 */
    ret = lseek(fd, file_header.offset, SEEK_SET);
    if(ret < 0) 
    {
        perror("lseek BMP error");
        close(fd);
        return -1;
    }

    /* 计算显示范围 */
    min_width = width > info_header.width ? info_header.width : width;
    min_height = height > info_header.height ? info_header.height : height;

    /* 显示图像数据 */
    int i, j;
    unsigned int color;
    if(info_header.bpp == 16)
    {
        line_buffer = (unsigned char *)malloc(min_width*2); //一行有效数据的缓冲区
        for(i = 0; i < min_height; i++)
        {
            /* 读取有效的一行数据 */
            ret = read(fd, line_buffer, min_width*2);
            if(ret < 0) 
            {   
                perror("read BMP error");
                free(line_buffer);
                close(fd);
                return -1;
            }
            for(j = 0; j < min_width; j++)
            {
                color = line_buffer[j*2+1]<<8 | line_buffer[j*2];
                if(direction_flag == 0)
                    *(fb_base + (min_height - 1 - i) * width + j) = color;
                else if(direction_flag == 1)
                    *(fb_base + i * width + j) = color;
            }
        }
    }
    else if(info_header.bpp == 24)
    {
        line_buffer = (unsigned char *)malloc(min_width*3); //一行有效数据的缓冲区
        for(i = 0; i < min_height; i++)
        {
            /* 读取有效的一行数据 */
            ret = read(fd, line_buffer, min_width*3);
            if(ret < 0) 
            {   
                perror("read BMP error");
                free(line_buffer);
                close(fd);
                return -1;
            }
            for(j = 0; j < min_width; j++)
            {
                color = line_buffer[j*3+2]<<16 | line_buffer[j*3+1]<<8 | line_buffer[j*3];
                if(direction_flag == 0)
                    *(fb_base + (min_height - 1 - i) * width + j) = argb8888_to_rgb565(color);
                else if(direction_flag == 1)
                    *(fb_base + i * width + j) = argb8888_to_rgb565(color);
            }
        }
    }
    else if(info_header.bpp == 32)
    {
        line_buffer = (unsigned char *)malloc(min_width*4); //一行有效数据的缓冲区
        for(i = 0; i < min_height; i++)
        {
            /* 读取有效的一行数据 */
            ret = read(fd, line_buffer, min_width*4);
            if(ret < 0) 
            {   
                perror("read BMP error");
                free(line_buffer);
                close(fd);
                return -1;
            }
            for(j = 0; j < min_width; j++)
            {
                color = line_buffer[j*4+3]<<24 | line_buffer[j*4+2]<<16 | line_buffer[j*4+1]<<8 | line_buffer[j*4];
                if(direction_flag == 0)
                    *(fb_base + (min_height - 1 - i) * width + j) = argb8888_to_rgb565(color);
                else if(direction_flag == 1)
                    *(fb_base + i * width + j) = argb8888_to_rgb565(color);
            }
        }
    }

    free(line_buffer);
    close(fd);
    return 0;
}



