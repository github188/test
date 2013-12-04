/*******************************************************************************
 * Author : liyunteng
 * Email : li_yunteng@163.com
 * Created Time : 2013-11-26 15:41
 * Filename : main.c
 * Description : 
 * *****************************************************************************/
#include <stdio.h>
#include "main.h"


static void sig_alarm(int signo)
{
	if (signo == SIGALRM) {
		now++;
		if (now == n)
			now = 0;
		strcpy(_dirname, root_dir);
		strcat(_dirname, "/");
		strcat(_dirname, dirs_name[now]);

		printf("now testing dir: %s\n",_dirname);
	}
}

int main(int argc, char *argv[])
{

	signal(SIGALRM, sig_alarm);

	if ((argc < 2) || (argc > 2 && argc < 5) || (argc > 5)) {
		fprintf(stderr, "Error args!\n");
		print_usage();
		return -1;
	}

	strcpy(root_dir, argv[1]);
	if (root_dir[strlen(root_dir)-1] == '/') {
		root_dir[strlen(root_dir)-1] = '\0';	
	}
	if (argc == 5) {
		file_size = atol(argv[2])*1024*1024;
		thread_n = atoi(argv[3]);
		time_s = atoi(argv[4]);
	}
	if ((n = parse_args()) <= 0) {
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "parse_args error!\n");
		print_usage();
		return -1;
	}
	strcpy(_dirname, root_dir);
	strcat(_dirname, "/");
	strcat(_dirname, dirs_name[now]);
	if (init_time(time_s) < 0) {
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "init_time error!\n");
		return -1;
	}

	if (w_thread() < 0) {
		return -1;
	}
	if (dfile() < 0) {
		return -1;
	}
}
