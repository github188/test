#ifndef __COMM
#define __COMM
#include <stdio.h>
#include <syslog.h>


#define BLOCK_SIZE 1024	/*buf大小*/
#define THREAD_MAX 32  /*最多线程个数*/

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
