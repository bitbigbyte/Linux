1. c语言可变参函数
2. 结构体内存对齐
3. 空洞文件
4. 函数指针
5. 堆，栈，数据段，代码段
6. C语言变成可执行文件需要经过预处理，编译，汇编和链接

```c
/* 延时函数 */
void delay(volatile int n) //注意：此处必须用volatile修饰，否则会被-O2优化，导致延时失效
{
   for (;n>0;n--)
    {
        
    }
}
```