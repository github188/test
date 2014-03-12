#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include "../common.h"

char *l_opt_arg;
char *const short_options = "i:s:d:h";
led_task_t task;
led_task_t systask;
static int disk_id = DISK_ID_NONE;
static int expire = TIME_FOREVER;
static int flags = 0; 		/* 检查是否为设置系统灯 */
struct option long_options[] = {
	{"sysled", 1, NULL, 's'},
	{"id", 1, NULL, 'i'},
	{"diskled", 1, NULL, 'd'},
	{"freq", 1, NULL, 0},
	{"expire", 1,&expire , 1},
	{"help", 1, NULL, 'h'},
	{0, 0, 0, 0}
};

void print_help(void)
{
	printf("tools-test-led-no-mcu:\n");
	printf("\t[--sysled|-s on|off\n");
	printf("\t[--diskled|-d on|off|[blink --freq fast|normal|slow]] [--id|-i <disk_id>|<all>]\n");
	printf("\t[--expire <seconds>]\n");
	printf("\t[--help|-h]\n");
}
int my_getopt(int argc, char **argv)
{
	int c;

	while ((c = getopt_long(argc, (char *const*)argv, short_options,
				long_options, NULL)) != -1) {
		switch (c) {
		case 's':
			flags = 1;
			if (!strcmp(optarg, "on")) {
				systask.mode = MODE_ON;
			} else if (!strcmp(optarg, "off")) {
				systask.mode = MODE_OFF;
			} else {
				systask.mode = MODE_NONE;
			}
			break;
		case 'i':
			if (!strcmp(optarg, "all")) {
				disk_id = DISK_ID_ALL;
			} else 
				disk_id = atoi(optarg);
			break;
		case 'd':
			if (!strcmp(optarg, "on")) {
				task.mode = MODE_ON;
				break;
			}
			if (!strcmp(optarg, "off")) {
				task.mode = MODE_OFF;
				break;
			}
			if (!strcmp(optarg, "blink")) {
				task.mode = MODE_BLINK;
				break;
			}
			
			fprintf(stderr, "diskled invalid.\n");
			print_help();
			return -1;
		case 'h':
			print_help();
			return -1;
		case 0:
			if (expire != TIME_FOREVER) {
				task.time = atol(optarg) * 1000;
				break;
			} else {
				if (!strcmp(optarg, "fast")) {
					task.freq = FREQ_FAST;
					break;
				}
				if (!strcmp(optarg, "normal")) {
					task.freq = FREQ_NORMAL;
					break;
				}
				if (!strcmp(optarg, "slow")) {
					task.freq = FREQ_SLOW;
					break;
				}
			}
			print_help();
			return -1;
		default:
			break;
		}
	}
	return 0;
}
int parse_args(void)
{
	if (task.mode != MODE_NONE && disk_id == DISK_ID_NONE) {
		fprintf(stderr, "please input disk id.\n");
		return -1;
	}
	if (task.mode == MODE_BLINK) {
		if (task.freq != FREQ_FAST && task.freq != FREQ_NORMAL
		    && task.freq != FREQ_SLOW) {
			fprintf(stderr, "blink freq invalid.\n");
			return -1;
		}
	}
	printf("disk_id:%d, mode:%d, time:%ld, freq:%d, count:%d\n",
	       disk_id, task.mode, task.time, task.freq, task.count);

	return 0;
	
}
int main(int argc, char *argv[])
{
	int shmid;
	key_t shmkey;
	shm_head_t *addr;
	led_task_t *taskp;
	
	if (argc < 3) {
		print_help();
		return -1;
	}
	task.freq = FREQ_NONE;
	task.mode = MODE_NONE;
	task.time = TIME_FOREVER;
	
	if (my_getopt(argc, argv))
		return -1;
	if (parse_args() < 0)
		return -1;
	
	shmkey = ftok(SHMKEY, 0);
	shmid = shmget(shmkey, 0, 0666);
	if (shmid == -1) {
		printf("get shm failed: %s.\n", strerror(errno));
		return -1;
	}
	
	addr = (shm_head_t *)shmat(shmid, 0, 0);
	if (addr == (shm_head_t *)-1) {
		printf("shmat failed.\n");
		return -1;
	}
	if (addr->magic != MAGIC) {
		printf("magic check failed.\n");
		return -1;
	}
	
	printf("disk_num:%d\n", addr->disk_num);
	if ( disk_id > addr->disk_num ||
	     ((disk_id < 0) && (disk_id != DISK_ID_NONE) && (disk_id != DISK_ID_ALL))
		) {
		printf("disk id invalid.\n");
		return -1;
	}
	
	/*系统灯*/
	taskp = (led_task_t *)((shm_head_t *)addr + 1);
	if (flags) {
		printf("done\n");
		taskp->mode = systask.mode;
		taskp->time = TIME_FOREVER;
		taskp->freq = FREQ_NONE;
		taskp->count = 0;
	}
	
	taskp++;
	if (disk_id == DISK_ID_ALL){
		int i;
		for (i=1; i <= addr->disk_num; i++){
			taskp->mode = task.mode;
			taskp->time = task.time;
			taskp->freq = task.freq;
			taskp->count = task.count;
			taskp++;
		}
	} else if (disk_id != DISK_ID_NONE){
		taskp = taskp + disk_id - 1;
		taskp->mode = task.mode;
		taskp->time = task.time;
		taskp->freq = task.freq;
		taskp->count = task.count;
	}

	return 0;
}
