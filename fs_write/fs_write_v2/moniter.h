#ifndef __DO_TEST_
#define __DO_TEST_
#include "comm.h"
/*#define DIRNAME "/mnt/sdb"*/


void release_percent(struct dirsname *);
int moniter(struct dirsname *, long file_size, int thread_n);
struct dirsname *get_fs_dirs(struct dirsname *);
#endif
