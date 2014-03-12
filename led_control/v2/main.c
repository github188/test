#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "common.h"
#include "led_shm.h"
#include "led_worker.h"
#include "pic_ctl/pic_ctl.h"

char *l_opt_arg;
char *const short_options = "t:c:h";
struct option long_options[] = {
	{"clean", 0, NULL, 'c'},
	{"type", 1, NULL, 't'},
	{"help", 0, NULL, 'h'},
	{0, 0, 0, 0}
};

extern shm_head_t *addr;
int disk_max_num = SHMSIZE;
int (*pic_write_disk_gen)(int, int);

void print_help(void)
{
	printf("led_server:\n");
	printf("\t[--type|-t 2U|3U]\n");
	printf("\t[--clean]\n");
	printf("\t[--help|-h]\n");
}

int main(int argc, char *argv[])
{
	int c;
	
	while ((c = getopt_long(argc, (char *const*)argv, short_options,
				long_options, NULL)) != -1) {
		switch (c){
		case 't':
			if (!strcmp(optarg, "3U")){
				disk_max_num = SHMSIZE;
				pic_write_disk_gen = pic_write_disk;
			} else if (!strcmp(optarg, "2U")) {
				disk_max_num = 8;
				pic_write_disk_gen = pic_write_disk_2U;
			} else {
				fprintf(stderr, "invalid type %s\n", optarg);
				print_help();
				return -1;
			}
			break;
		case 'c':
			goto clean;
		default:
			print_help();
			return -1;
		}
	}
	
	int shmid;
	shmid = shm_init();
	if (shmid < 0)
		return -1;
	worker_init();

	worker_release();
clean:	
	shm_release();
	return 0;
}
