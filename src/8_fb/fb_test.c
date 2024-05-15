#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#define DEBUG
#ifdef DEBUG
#define DBG(...) fprintf(stderr, " DBG(%s, %s(), %d): ", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif            
#define FB_DEV	"/dev/fb0"

unsigned int width;        //LCD X分辨率
unsigned int height;       //LCD Y分辨率
unsigned int bpp;          //LCD像素深度
unsigned short *fb_base;   //映射后的显存基地址
unsigned int fb_size;	   //映射后的显存大小
void lcd_draw_line(void);

int main(int argc, char **argv)
{
    int fd;
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;

    /* 打开framebuffer设备 */
	fd = open(FB_DEV, O_RDWR);
    if(fd < 0) 
	{
        perror("open error");
        return -1;
    }

    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);  //可变信息
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);  //固定信息
    fb_size = fb_fix.line_length * fb_var.yres;
    width = fb_var.xres;
    height = fb_var.yres;
    bpp = fb_var.bits_per_pixel;
    DBG("LCD分辨率: %d*%d\n", width, height);
	DBG("LCD像素深度: %d\n", bpp);


    /* 将显示缓冲区映射到进程地址空间 */
    fb_base = mmap(NULL, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if((void *)fb_base == MAP_FAILED) 
	{
        perror("mmap error");
        close(fd);
        return -1;
    }

    /* 画线 */
	memset(fb_base, 0xff, fb_size); //LCD设置为白色背景
	lcd_draw_line();

    /* 退出 */
    munmap(fb_base, fb_size);  //取消映射
    close(fd);  			   //关闭文件
    return 0;    			   //退出进程
}

void lcd_draw_line(void)
{
	int i, j;
	for(j = 160; j < 480; j++)
	{
		for(i = 500; i < 530; i++)
		{
			*(fb_base + j * width + i) = 0x00; //填充为黑色
		}
	}
}