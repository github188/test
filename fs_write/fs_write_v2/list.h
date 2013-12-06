#ifndef __LIST_H
#define __LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comm.h"

int list_empty(struct dirsname *);
struct dirsname *list_end(struct dirsname *);
int list_have(struct dirsname*, char *);
int list_add(struct dirsname *, char *);
int list_del(struct dirsname *, char *);
struct dirsname *list_next(struct dirsname *, struct dirsname *);
#endif
