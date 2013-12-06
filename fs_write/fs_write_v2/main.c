/*******************************************************************************
 * Author : liyunteng
 * Email : li_yunteng@163.com
 * Created Time : 2013-11-26 15:41
 * Filename : main.c
 * Description : 
 * *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "comm.h"
#include "parse_args.h"
#include "moniter.h"
#include "threads.h"
#include "main.h"

char *l_opt_arg;
char *const short_options = "s:b:n:t:p:h";
struct option long_options[] = {
	{"file-size", 1, NULL, 's'},
	{"block-size", 1, NULL, 'b'},
	{"threadn", 1, NULL, 'n'},
	{"time", 1, NULL, 't'},
	{"choose-policy", 1, NULL, 'p'},
	{"help", 0, NULL, 'h'},
	{0,0,0,0}
};

long file_size = 268435456;
int block_size = 1024;
int thread_n = 4;
int time_s=0;
char root_dir[300];


int main(int argc, char *argv[])
{

	struct dirsname *dirsp=NULL;
	int c;

	if ((argc < 2)) {
		fprintf(stderr, "No root_dir!\n");
		print_help();
		return -1;
	}
	strcpy(root_dir, argv[1]);
	if (root_dir[strlen(root_dir)-1] == '/') {
		root_dir[strlen(root_dir)-1] = '\0';	
	}
	while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch(c) {
			case 's':
				file_size = atol(optarg)*1024*1024;
				break;
			case 'b':
				block_size = atoi(optarg);
				break;
			case 'n':
				thread_n = atoi(optarg);
				break;
			case 't':
				time_s = atoi(optarg);
				break;
			case 'p':
				break;
			case 'h':
				print_usage();
				return 0;
				break;
			default :
				print_help();
				return -1;
				break;
		}

	}
	/*printf("file_size:%ld\nblock_size:%d\nthread_n:%d\ntime_s:%d\n", file_size,
	  block_size, thread_n, time_s); */
	dirsp=(struct dirsname *)malloc(sizeof(struct dirsname));
	if (dirsp == NULL) {
		fprintf(stderr, "malloc dirsp error!\n");
		return -1;
	}
	dirsp->name[0] = '\0';
	dirsp->next = dirsp;
	if (chdir(root_dir) < 0) {
		fprintf(stderr, "root_dir is invalid!\n");
		print_help();
		return -1;
	}

	if (parse_args(dirsp, file_size, thread_n) < 0) {
		//fprintf(stderr, "parse_args error!\n");
		print_help();
		return -1;
	}
	if (start_w_thread(dirsp, file_size, block_size, thread_n, time_s) < 0) {
		return -1;
	}
	if (moniter(dirsp, file_size, thread_n) < 0) {
		return -1;
	}

}
