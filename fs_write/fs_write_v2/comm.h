#ifndef __COMM
#define __COMM
#include <stdio.h>
#include <syslog.h>


#define BLOCK_SIZE 1024	/*buf大小*/
#define FILE_SIZE 268435456  /*要写的每个文件大小为256M*/
#define THREAD_N 4 /*写线程个数*/
#define THREAD_MAX 32  /*最多线程个数*/
#define TIME 600	/*更改文件系统的时间间隔*/

extern long int file_size;
extern int thread_n;
extern int time_s;

struct dirsname{
	char name[256];
	struct dirsname *next;
	//struct list *list;
};

extern struct dirsname *dirsp;
char root_dir[300];
#endif
