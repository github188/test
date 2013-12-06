/*******************************************************************************
 * Author : liyunteng
 * Email : li_yunteng@163.com
 * Created Time : 2013-11-28 11:25
 * Filename : init_time.c
 * Description : 
 * *****************************************************************************/
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include "init_time.h"

/*定时发送SIGALRM信号*/
int init_time(int tim)
{
	struct itimerval value;
	value.it_value.tv_sec = tim;
	value.it_value.tv_usec = 0;
	value.it_interval = value.it_value;

	if (setitimer(ITIMER_REAL, &value, NULL) < 0) {
		fprintf(stderr, "Setitimer error!\n");
		return -1;
	}
	return 0;
}
