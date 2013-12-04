#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/vfs.h>
#include "comm.h"


int check_fs(int n);
int get_dirs(void);
void print_usage(void);
int check_threadn(void);
int parse_args(void);
