#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/statfs.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include "parse_args.h"
#include "comm.h"
#include "mdigest.h"
#include "list.h"

#define FILE_MOD S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
/*#define DIRNAME "/mnt/sdb"*/


pthread_t threads[THREAD_MAX];
struct thread_args{
	int thread_num;
	struct dirsname *dp;
};

int get_random(void);
void *_w_thread(void *arg);
int w_thread(struct dirsname *); 
int get_time(char *buf);
int get_date(char *buf);
int get_hour(char *buf);
int do_test(char *dirname);
int find_delete(char *dir, char *buf);
int _dfile(char *dir);
int dfile(struct dirsname *);
