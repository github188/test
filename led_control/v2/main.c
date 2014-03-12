#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "common.h"
#include "led_shm.h"
#include "led_worker.h"
#include "pic_ctl/pic_ctl.h"

char *l_opt_arg;
char *const short_options = "t";
struct option long_options[] = {
	{"type", 1, NULL, 't'},
	{"clean", 0, NULL, 'c'},
	{0, 0, 0, 0}
};

int disk_max_num;
extern shm_head_t *addr;
int (*pic_write_disk_gen)(int, int);

int main(int argc, char *argv[])
{
	int c;
	disk_max_num = SHMSIZE;
	pic_write_disk_gen = pic_write_disk;

	while ((c = getopt_long(argc, (char *const*)argv, short_options,
				long_options, NULL)) != -1) {
		switch (c){
		case 't':
			if (!strcmp(optarg, "3U")){
				disk_max_num = 16;
				pic_write_disk_gen = pic_write_disk;
			} else if (!strcmp(optarg, "2U")) {
				pic_write_disk_gen = pic_write_disk_2U;
			} else {
				disk_max_num = 8;
				fprintf(stderr, "invalid type %s\n", optarg);
				return -1;
			}
			break;
		case 'c':
			worker_release();
			shm_release();
			return 0;
		default:
			break;
		}
	}
	int shmid;
	shmid = shm_init();
	if (shmid < 0)
		return -1;
	worker_init();

	
	worker_release();
	shm_release();
	return 0;
}
