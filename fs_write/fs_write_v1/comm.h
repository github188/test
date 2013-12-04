#ifndef __COMM
#define __COMM
#include <stdio.h>
#include <syslog.h>


#define MAX_DIRS 20  /*root_dir目录下最大子目录个数*/
#define BLOCK_SIZE 1024	/*buf大小*/
#define FILE_SIZE 536870912  /*要写的每个文件大小为500M*/
#define THREAD_N 4 /*写线程个数*/
#define THREAD_MAX 32  /*最多线程个数*/
#define TIME 600	/*更改文件系统的时间间隔*/



extern long int file_size;
extern int thread_n;
extern int time_s;

char root_dir[300];
char dirs_name[MAX_DIRS][256];
char _dirname[500];
#endif
