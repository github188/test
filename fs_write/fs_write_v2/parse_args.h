#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/vfs.h>
#include "comm.h"
#include "list.h"

int check_fs(char *root_dir, struct dirsname *);
int get_dirs(char *root_dir, struct dirsname *);
void print_usage(void);
int check_threadn(void);
long get_fs_space(char *);
int get_fs_stat(char *);
struct dirsname * get_fs_dirs(struct dirsname *);
int parse_args(char *root_dir,struct dirsname *dirsp);
