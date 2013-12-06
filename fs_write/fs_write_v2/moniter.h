#ifndef __DO_TEST_
#define __DO_TEST_
#include "comm.h"
/*#define DIRNAME "/mnt/sdb"*/


/*pthread_t threads[THREAD_MAX];*/
struct thread_args{
	long file_size;
	int block_size;
	int thread_num;
	int time_s;
	struct dirsname *dp;
};

int moniter(struct dirsname *, long file_size, int thread_n);
struct dirsname *get_fs_dirs(struct dirsname *);
#endif
