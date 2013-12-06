#ifndef __COMM
#define __COMM
#include <stdio.h>
#include <syslog.h>


#define THREAD_MAX 32  /*最多线程个数*/
#define START_RELEASE 20.0 /*开始删除的可用空间比*/
#define END_RELEASE 40.0 /*删除到60%*/


struct dirsname{
	char name[256];
	struct dirsname *next;
	//struct list *list;
};

extern char root_dir[300];
#endif
