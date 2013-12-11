#ifndef __PARSE_ARGS_
#define __PARSE_ARGS_
#include "comm.h"

void print_help(void);
void print_usage(void); /*打印帮助*/
void print_dirsp(struct dirsname *);
int update_list(struct dirsname *, long, int); /*更新文件系统链表*/
int parse_args(struct dirsname *, long, int, int, char *, int *); /*解析参数*/
#endif
