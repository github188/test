#ifndef __LIST_H
#define __LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comm.h"

int fs_list_empty(struct dirsname *);
struct dirsname *fs_list_end(struct dirsname *);
int fs_list_have(struct dirsname*, char *);
int fs_list_add(struct dirsname *, char *);
int fs_list_del(struct dirsname *, char *);
struct dirsname *fs_list_next(struct dirsname *, struct dirsname *);
#endif
