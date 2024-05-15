#include "main.h"
#include "show_bmp.h"
#include "show_jpeg.h"
#include "show_png.h"

#define PATHNMAE_LEN        256
#define MAX_IMAGE_CNT       1000

typedef enum image_type
{
	IMAGE_TYPE_BMP,
	IMAGE_TYPE_JPG,
	IMAGE_TYPE_PNG,
	IMAGE_TYPE_UNKNOW,	
}image_type_e;

typedef struct image_info
{
	char pathname[PATHNMAE_LEN];//图片文件的pathname
	image_type_e type;			//图片文件的格式		
}image_info_t;

image_info_t images[MAX_IMAGE_CNT]; //定义存储图片信息的数组
int max_index; 
int image_index;
int k;					
unsigned int width;        //LCD X分辨率
unsigned int height;       //LCD Y分辨率
unsigned short *fb_base;   //映射后的显存基地址
unsigned int fb_size;	   //映射后的显存大小
unsigned int bpp;          //LCD像素深度
image_type_e judge_image_type(const char *path);
int scan_image(const char *path);
int show_image(int index);
void index_limit(int *index, int flag);

int main(int argc, char *argv[])
{
	int ret;
    struct tsdev *ts;
    struct ts_sample_mt *mt_ptr;
    struct input_absinfo slot;
    int max_slots;
    unsigned int pressure[12] = {0};   //用于保存每一个触摸点上一次的按压力,初始为0,表示松开
	int fd;
	struct fb_var_screeninfo fb_var;   
	struct fb_fix_screeninfo fb_fix;

	/* 校验传参 */
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <dir>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

	ret = scan_image(argv[1]);
	if(ret < 0)
	{
		fprintf(stderr, "scan image error!\n");
		exit(EXIT_FAILURE);
	}

     /* 打开framebuffer设备 */
	fd = open(FB_DEV, O_RDWR);
	if(fd < 0) 
	{
		perror("open LCD error");
		exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

	/* 映射LCD显存 */
	fb_base = mmap(NULL, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //映射
	if((void*)fb_base == MAP_FAILED) 
	{
        perror("mmap LCD error\n");
		close(fd);
        exit(EXIT_FAILURE);
    }
    
    /* 显示1张图片 */
	memset(fb_base, 0xff, fb_size); //刷白屏幕
	max_index = image_index - 1;
	image_index = max_index / 2;
	DBG("max_index: %d\n", max_index);
	DBG("current_index: %d\n", image_index);
    ret = show_image(image_index);
    if(ret < 0)
    {
        fprintf(stderr, "show image error!\n");
		munmap(fb_base, fb_size);
		close(fd);
        exit(EXIT_FAILURE);
    }


    /* 打开并配置触摸屏设备 */
    ts = ts_setup(NULL, 0);
    if(ts == NULL) 
    {
        fprintf(stderr, "ts_setup error!\n");
        exit(EXIT_FAILURE);
    }

    /* 获取触摸屏支持的最大触摸点数 */
    ret = ioctl(ts_fd(ts), EVIOCGABS(ABS_MT_SLOT), &slot);
    if(ret < 0) 
    {
        perror("ioctl error");
        ts_close(ts);
        exit(EXIT_FAILURE);
    }
    max_slots = slot.maximum + 1 - slot.minimum;

    /* 内存分配 */
    mt_ptr = calloc(max_slots, sizeof(struct ts_sample_mt));

    /* 读数据 */
    while(1)
    {
        ret = ts_read_mt(ts, &mt_ptr, max_slots, 1);
        if(ret < 0) 
        {
            perror("ts_read_mt error");
            free(mt_ptr);
            ts_close(ts);
            exit(EXIT_FAILURE);
        }

        int i;
        int last_x;
        int flag;
        for(i = 0; i < max_slots; i++) 
        {
            if(mt_ptr[i].valid) //有效表示有更新
            {
                if(mt_ptr[i].pressure) //如果按压力>0
                { 
                    if(pressure[mt_ptr[i].slot]) //如果上一次的按压力>0
                    {
                        if(mt_ptr[i].x > last_x && flag == 0)
						{
							DBG("slot<%d>, 向右滑动\n", mt_ptr[i].slot);
							index_limit(&image_index, 1);
							DBG("current_index: %d\n", image_index);
							memset(fb_base, 0xff, fb_size); //刷白屏幕
							show_image(image_index);
						}
                        else if(mt_ptr[i].x < last_x && flag == 0)
						{
                            DBG("slot<%d>, 向左滑动\n", mt_ptr[i].slot);
							index_limit(&image_index, -1);
							DBG("current_index: %d\n", image_index);
							memset(fb_base, 0xff, fb_size); //刷白屏幕
							show_image(image_index);
						}
                        flag = 1;
                    }
                    else
                        flag = 0;
                    last_x = mt_ptr[i].x;
                }
                pressure[mt_ptr[i].slot] = mt_ptr[i].pressure;
            }
        }
    }

    /* 关闭设备、释放内存、退出 */
    free(mt_ptr);
    ts_close(ts);
   	munmap(fb_base, fb_size);   
    close(fd);                  
    exit(EXIT_SUCCESS);
}

image_type_e judge_image_type(const char *path) 
{  
	int fd;
	int ret;
	unsigned char buffer[8];

    fd = open(path, O_RDWR);  
    if(fd < 0) 
	{  
        perror("open error");  
        return IMAGE_TYPE_UNKNOW;  
    }  
	
    ret = read(fd, buffer, sizeof(buffer));
	close(fd);

    if(buffer[0] == 0x42 && buffer[1] == 0x4D)
        return IMAGE_TYPE_BMP;  
    else if(buffer[0] == 0xFF && buffer[1] == 0xD8)
        return IMAGE_TYPE_JPG;  
    else if(ret >= 8 &&  buffer[0] == 0x89 && buffer[1] == 0x50 && 
		buffer[2] == 0x4E && buffer[3] == 0x47 && buffer[4] == 0x0D &&
		buffer[5] == 0x0A && buffer[6] == 0x1A && buffer[7] == 0x0A) 
		return IMAGE_TYPE_PNG;
	else  
		return IMAGE_TYPE_UNKNOW;  
}  

//把文件夹的所有图片填充到iamges数组中去
int scan_image(const char *path)
{
	DIR *dir;
	struct dirent *file_ptr;
	char base_path[256];
	struct stat sta;//文件属性相关的结构体

    //打开文件夹
    dir = opendir(path);
	if(dir == NULL)
	{
		perror("open dir error");
		return -1;
	}

	//readdir函数每调用一次就会返回opendir打开的basepath目录下的一个文件，
    //直到basepath目录下所有文件都被读完之后，就会返回NULL
	while((file_ptr = readdir(dir)) != NULL)
	{
        //排除两个特殊的文件
		if(strcmp(file_ptr->d_name, ".") == 0 || strcmp(file_ptr->d_name, "..") == 0) 
			continue;
		
		sprintf(base_path, "%s/%s", path, file_ptr->d_name);
		lstat(base_path, &sta);

        //不同的文件类型分别处理
		if(S_ISREG(sta.st_mode)) //普通文件
		{
			if(judge_image_type(base_path) == IMAGE_TYPE_UNKNOW)
			{
				continue;
			}
			else if(judge_image_type(base_path) == IMAGE_TYPE_BMP)
			{
				images[image_index].type = IMAGE_TYPE_BMP;
				strcpy(images[image_index].pathname, base_path);
			}
			else if(judge_image_type(base_path) == IMAGE_TYPE_JPG)
			{
				images[image_index].type = IMAGE_TYPE_JPG;
				strcpy(images[image_index].pathname, base_path);
			}
			else if(judge_image_type(base_path) == IMAGE_TYPE_PNG)
			{
				images[image_index].type = IMAGE_TYPE_PNG;
				strcpy(images[image_index].pathname, base_path);
			}
			image_index++;
			
		}
		else if(S_ISDIR(sta.st_mode)) //文件夹
		{
			scan_image(base_path);
		}
	}
}

int show_image(int index)
{
	int ret= 0;
	if(images[index].type == IMAGE_TYPE_UNKNOW)
		ret = -1;
	else if(images[index].type == IMAGE_TYPE_BMP)
		show_bmp(images[index].pathname);
	else if(images[index].type == IMAGE_TYPE_JPG)
		show_jpeg(images[index].pathname);
	else if(images[index].type == IMAGE_TYPE_PNG)
		show_png(images[index].pathname);
		
	return ret;	
}

void index_limit(int *index, int flag)
{
	if(flag < 0)
	{
		*index-=1;
		*index = *index < 0 ? max_index : *index;
	}
	else if(flag > 0)
	{
		*index+=1;
		*index = *index > max_index ? 0 : *index;
	}
}
