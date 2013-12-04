/*******************************************************************************
 * Author : liyunteng
 * Email : li_yunteng@163.com
 * Created Time : 2013-11-26 15:41
 * Filename : main.c
 * Description : 
 * *****************************************************************************/
#include "main.h"

int main(int argc, char *argv[])
{

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

	dirsp=(struct dirsname *)malloc(sizeof(struct dirsname));
	if (dirsp == NULL) {
		fprintf(stderr, "malloc dirsp error!\n");
		return -1;
	}
	dirsp->name[0] = '\0';
	dirsp->next = dirsp;
	if (chdir(root_dir) < 0) {
		fprintf(stderr, "Can't into the root_dir!\n");
		return -1;
	}
	if (parse_args(root_dir,dirsp) < 0) {
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "parse_args error!\n");
		print_usage();
		return -1;
	}
	if (w_thread(dirsp) < 0) {
		return -1;
	}
	if (dfile(dirsp) < 0) {
		return -1;
	}

}
