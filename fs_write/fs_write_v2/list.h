#ifndef __LIST_H
#define __LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comm.h"

struct dirsname *list_end(struct dirsname *);
int list_have(struct dirsname*, char *);
int list_add(struct dirsname *, char *);
int list_del(struct dirsname *, char *);

#endif
