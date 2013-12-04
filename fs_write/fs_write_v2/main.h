#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "comm.h"
#include "parse_args.h"
#include "do_test.h"


struct dirsname *dirsp=NULL;
long int file_size = 524288000;
int thread_n = 4;
int time_s = 120;

static int  n, now=0;
