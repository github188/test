#ifndef __DO_TEST_
#define __DO_TEST_
#include "comm.h"
/*#define DIRNAME "/mnt/sdb"*/


int release_percent(char *);
int moniter(struct dirsname *, long file_size, int thread_n);
struct dirsname *get_fs_dirs_by_percent(struct dirsname *, int, long);
struct dirsname *get_fs_dirs_by_size(struct dirsname *, int, long);
struct dirsname *get_fs_dirs_by_weight(struct dirsname *, int, long);
struct dirsname *get_fs_dirs_default(struct dirsname *,struct dirsname *, int, long);
#endif
