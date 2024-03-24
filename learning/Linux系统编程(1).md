[Linux系统编程(1)](./Linux系统编程(1).md)
[Linux系统编程(2)](./Linux系统编程(2).md)

[TOC]

# 1.文件I/O:
## 1.1普通I/O:
### access,open,read,write,lseek,ioctl,dup,dup2,close
## 1.2标准I/O:
### fopen,fread,fwrite,fseek,fclose

# 2.字符串处理：
## strlen,strcmp,strcat,strcpy,memset,strchr,strstr,atoi,itoa,正则表达式

# 3.输入输出流和错误流：
## printf,sprintf,fprintf,scanf,gets,puts,perror,getchar,putchar

# 4.linux中文件类型：
## ' - '：普通文件
## ' d '：目录文件
## ' c '：字符设备文件
## ' b '：块设备文件
## ' l '：符号链接文件
## ' s '：套接字文件
## ' p '：管道文件

# 5.杂项：
## 5.1随机数：
### rand,srand
## 5.2休眠：
### sleep,usleep
## 5.3申请堆内存(动态内存分配)：
### malloc,calloc,realloc,free

# 6.信号：
## 6.1不可靠信号(非实时信号，标准信号)：
### 6.1.1在 Linux 系统下，信号值小于 SIGRTMIN(34) 的信号都是不可靠信号
### 6.1.2设置信号处理方式：
#### signal,sigaction
### 6.1.3向进程发送信号：
#### kill
### 6.1.4设置定时器：
#### alarm
## 6.2可靠信号(实时信号)：
### SIGRTMIN - SIGRTMAX，可靠信号支持排队，不会丢失