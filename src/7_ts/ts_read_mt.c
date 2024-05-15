#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <tslib.h>

int main(int argc, char *argv[])
{
    int ret;
    struct tsdev *ts;
    struct ts_sample_mt *mt_ptr;
    struct input_absinfo slot;
    int max_slots;
    unsigned int pressure[12] = {0};   //用于保存每一个触摸点上一次的按压力,初始为0,表示松开

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
    printf("max_slots: %d\n", max_slots);

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
        for(i = 0; i < max_slots; i++) 
        {
            if(mt_ptr[i].valid) //有效表示有更新
            {
                if(mt_ptr[i].pressure) //如果按压力>0
                { 
                    if(pressure[mt_ptr[i].slot]) //如果上一次的按压力>0
                        printf("slot<%d>, 移动(%d, %d)\n", mt_ptr[i].slot, mt_ptr[i].x, mt_ptr[i].y);
                    else
                        printf("slot<%d>, 按下(%d, %d)\n", mt_ptr[i].slot, mt_ptr[i].x, mt_ptr[i].y);
                }
                else
                    printf("slot<%d>, 松开\n", mt_ptr[i].slot);

                pressure[mt_ptr[i].slot] = mt_ptr[i].pressure;
            }
        }
    }

    /* 关闭设备、释放内存、退出 */
    free(mt_ptr);
    ts_close(ts);
    exit(EXIT_SUCCESS);
}