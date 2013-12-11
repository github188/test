#ifndef __COMM
#define __COMM
#include <stdio.h>
#include <syslog.h>


#define THREAD_MAX 32  /*最多线程个数*/
#define START_RELEASE 0.2 /*开始删除的可用空间比*/
#define END_RELEASE 0.4 /*删除到60%*/


struct dirsname{
	char name[256];
	struct dirsname *next;
	int weight;
	//struct list *list;
};

extern char root_dir[300];
#endif
