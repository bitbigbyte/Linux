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
#include <png.h>

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
#define FB_DEV  "/dev/fb0"

typedef struct png
{
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
}png_t;

unsigned int width;        //LCD X分辨率
unsigned int height;       //LCD Y分辨率
unsigned short *fb_base;   //映射后的显存基地址
unsigned int fb_size;	   //映射后的显存大小
unsigned int bpp;          //LCD像素深度
int show_png(const char *path);

int main(int argc, char *argv[])
{
    int fd;
    int ret;
    struct fb_var_screeninfo fb_var;
    struct fb_fix_screeninfo fb_fix;

    /* 校验传参 */
    if(argc != 2) 
    {
        fprintf(stderr, "Usage: %s <png_file>\n", argv[0]);
        return -1;
    }

    /* 打开framebuffer设备 */
    fd = open(FB_DEV, O_RDWR);
    if(fd < 0) 
    {
        perror("open LCD error");
        return -1;
    }

    /* 获取LCD信息 */
	ioctl(fd, FBIOGET_VSCREENINFO, &fb_var); //获取屏幕可变信息
	ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix); //获取屏幕固定信息
	width = fb_var.xres;
	height = fb_var.yres;
	fb_size = fb_fix.line_length * height;
	bpp = fb_var.bits_per_pixel;
	DBG("LCD分辨率: %d*%d\n", width, height);
	DBG("LCD像素深度: %d\n", bpp);
	if(bpp != 16)
    {
        fprintf(stderr, "This program only supports LCD with a bpp of 16!\n");
        close(fd);
        return -1;
    }

    /* 映射LCD显存 */
	fb_base = mmap(NULL, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //映射
	if((void*)fb_base == MAP_FAILED) 
	{
        perror("mmap LCD error\n");
		close(fd);
        return -1;
    }

    /* 显示png图片 */
	memset(fb_base, 0xff, fb_size); //刷白屏幕
    ret = show_png(argv[1]);
    if(ret < 0)
    {
        fprintf(stderr, "show png error!\n");
		munmap(fb_base, fb_size);
		close(fd);
        return -1;
    }

    /* 退出 */
    munmap(fb_base, fb_size);   //取消映射
    close(fd);                  //关闭文件
    return 0;                   //退出进程
}

int show_png(const char *path)
{
    FILE *png_file;
    png_t png;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytepp row_pointers;
    unsigned int min_height;
	unsigned int min_width;

    /* 打开png文件 */
    png_file = fopen(path, "r");    //以只读方式打开
    if(png_file == NULL) 
    {
        perror("fopen png error");
        return -1;
    }

    /* 分配和初始化png_ptr、info_ptr */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(png_ptr == NULL) 
    {
        fclose(png_file);
        return -1;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL) 
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(png_file);
        return -1;
    }

    /* 设置错误处理函数 */
    if(setjmp(png_jmpbuf(png_ptr))) 
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        return -1;
    }

    /* 指定数据源 */
    png_init_io(png_ptr, png_file);

    /* 读取png文件 */
    //PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_STRIP_ALPHA 这两个标志是使png图像数据转换为标准的BGR24格式
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_STRIP_ALPHA, NULL);
    png.width = png_get_image_width(png_ptr, info_ptr);
    png.height = png_get_image_height(png_ptr, info_ptr);
    if((png_get_bit_depth(png_ptr, info_ptr) != 8) || (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGB))
    {
        fprintf(stderr, "This image cannot be converted to BGR24 format!\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        return -1;
    }
    png.bpp = 24;
    DBG("png图像分辨率: %d*%d\n", png.width, png.height);
    DBG("png图像像素深度: %d\n", png.bpp);

    //计算显示范围
    min_width = width > png.width ? png.width : width;
    min_height = height > png.height ? png.height : height;

    /* 读取解码后的数据 */
    int i, j;
    unsigned int color;
    row_pointers = png_get_rows(png_ptr, info_ptr); //获取数据
    for(i = 0; i < min_height; i++)
    {
        for(j = 0; j < min_width; j++)
        {
            color = row_pointers[i][j*3]<<16 | row_pointers[i][j*3+1]<<8 | row_pointers[i][j*3+2];
            *(fb_base + i * width + j) = argb8888_to_rgb565(color);
        }
    }

    /* 结束 */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(png_file);
    return 0;
}