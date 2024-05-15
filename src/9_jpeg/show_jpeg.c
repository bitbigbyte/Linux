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
#include <jpeglib.h>

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

unsigned int width;        //LCD X分辨率
unsigned int height;       //LCD Y分辨率
unsigned short *fb_base;   //映射后的显存基地址
unsigned int fb_size;	   //映射后的显存大小
unsigned int bpp;          //LCD像素深度
int show_jpeg(const char *path);

int main(int argc, char **argv)
{
	int fd;
    int ret;
	struct fb_var_screeninfo fb_var;   
	struct fb_fix_screeninfo fb_fix;

	/* 校验传参 */
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <jpeg_file>\n", argv[0]);
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
    
    /* 显示jpeg图片 */
	memset(fb_base, 0xff, fb_size); //刷白屏幕
    ret = show_jpeg(argv[1]);
    if(ret < 0)
    {
        fprintf(stderr, "show jpeg error!\n");
		munmap(fb_base, fb_size);
		close(fd);
        return -1;
    }

    /* 退出 */
    munmap(fb_base, fb_size);   //取消映射
    close(fd);                  //关闭文件
    return 0;                   //退出进程
}

int show_jpeg(const char *path)
{
    FILE *jpeg_file;
	struct jpeg_decompress_struct cinfo; //解压对象
	struct jpeg_error_mgr jerr; 		 //错误处理对象
	unsigned int min_height;
	unsigned int min_width;
	unsigned char *line_buffer;		 	 //行缓冲区:用于存储从jpeg文件中解压出来的一行图像数据

	//打开jpeg图像文件
    jpeg_file = fopen(path, "r");  //只读方式打开
    if(jpeg_file == NULL) 
    {
        perror("fopen jpeg error");
        return -1;
    }

	//错误处理对象与解压对象绑定
	cinfo.err = jpeg_std_error(&jerr); 
	//创建解码对象
	jpeg_create_decompress(&cinfo);
	//指定解码数据源
	jpeg_stdio_src(&cinfo, jpeg_file);
	//读取图像信息
	jpeg_read_header(&cinfo, TRUE);
	DBG("jpeg图像分辨率: %d*%d\n", cinfo.image_width, cinfo.image_height);
	//设置解码参数，默认为RGB
	cinfo.out_color_space = JCS_RGB;
	//设置图像缩放:scale_num/scale_denom为缩放比例，默认为1
	cinfo.scale_num = 1;
	cinfo.scale_denom = 1;
	//开始解码
	jpeg_start_decompress(&cinfo);
    DBG("jpeg图像像素深度: %d\n", cinfo.output_components * 8);

	//为缓冲区分配空间
	line_buffer = (unsigned char *)malloc(cinfo.output_components * cinfo.output_width);
	//计算显示范围
    min_width = width > cinfo.image_width ? cinfo.image_width : width;
    min_height = height > cinfo.image_height ? cinfo.image_height : height;
	//读取数据，数据按行读取
	int i;
    unsigned int color;
	while(cinfo.output_scanline < min_height) 
	{
		jpeg_read_scanlines(&cinfo, &line_buffer, 1); //每次读取一行
		for(i = 0; i < min_width; i++) //读取到的数据是BGR形式（不知道的话可以按照RGB的形式写,如果颜色不正确反过来就行）
		{
			if(cinfo.output_components * 8 == 16)
			{
				color = line_buffer[i*2]<<8 | line_buffer[i*2+1];
				//注意这里要减1,因为cinfo.output_scanline在每次被读取的时候才会加1，而读取在最前面会导致这里的cinfo.output_scanline永远比循环判断条件里的大1，不减1会越界访问，所以要减1
				*(fb_base + (cinfo.output_scanline - 1) * width + i) = color; 
			}
			else if(cinfo.output_components * 8 == 24)
			{
				color = line_buffer[i*3]<<16 | line_buffer[i*3+1]<<8 | line_buffer[i*3+2];
				*(fb_base + (cinfo.output_scanline - 1) * width + i) = argb8888_to_rgb565(color);
			}
			else if(cinfo.output_components * 8 == 32)
			{
				color = line_buffer[i*4]<<24 | line_buffer[i*4+1]<<16 | line_buffer[i*4+2]<<8 | line_buffer[i*4+3];
				*(fb_base + (cinfo.output_scanline - 1) * width + i) = argb8888_to_rgb565(color);
			}
		}
	}

	//完成解码
	jpeg_finish_decompress(&cinfo);
	//销毁解码对象
	jpeg_destroy_decompress(&cinfo);
	//释放内存
	free(line_buffer);
	//关闭文件
	fclose(jpeg_file);
	return 0;
}
