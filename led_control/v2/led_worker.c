#include <stdio.h>
#include <ev.h>
#include "common.h"
#include "led_worker.h"
#include "pic_ctl/pic_ctl.h"


extern int disk_max_num;

extern shm_head_t *addr;
extern int (*pic_write_disk_gen)(int, int);
static ev_timer worker_timer;
static struct ev_loop *led_loop = NULL;
static int sts[SHMSIZE + 1];

static void timer_cb(EV_P_ ev_timer *w, int r)
{
	led_task_t *taskp = NULL;
	led_task_t *systaskp = NULL;
	int i;

	/* 跳过shm的头部 */
	systaskp = (led_task_t *)((shm_head_t *)addr + 1);

	/* 检查系统灯 */
	if (systaskp->mode & MODE_ON) {
		//TODO点亮系统灯
		sb_gpio28_set(1);
		printf("sysled on\n");
	} else if (systaskp->mode & MODE_OFF) {
		//TODO熄灭系统灯
		sb_gpio28_set(0);
		printf("sysled off\n");
	}

	for (i=0; i < disk_max_num; i++) {
		taskp = systaskp + i + 1;
		if (taskp == NULL) {
			fprintf(stderr, "taskp is null\n");
			continue;
		}
		if (taskp->mode == MODE_NONE) {
			fprintf(stderr, "mode not set.\n");
			continue;
		}

		if (taskp->time <= 0 && taskp->time != TIME_FOREVER) {
			taskp->mode = MODE_NONE;
			continue;
		}
		
		if (taskp->mode & MODE_ON) {
			if (pic_write_disk_gen(i, PIC_LED_ON) != 0) {
				fprintf(stderr, "led on disk %d failed.", i);
			} else 
				printf("led on done\n");
		} else if (taskp->mode & MODE_OFF) {
			if (pic_write_disk_gen(i, PIC_LED_OFF) != 0) {
				fprintf(stderr, "led off disk %d failed.", i);
			} else 
				printf("led off done\n");
		} else if (taskp->mode & MODE_BLINK) {
			if (taskp->freq == FREQ_NONE) {
				fprintf(stderr, "disk %d freq not set.", i);
				continue;
			} 
			
			 if (taskp->count == 0) {
				if (pic_write_disk_gen(i, sts[i+1]) != 0) {
					fprintf(stderr, "blink disk %d failed.", i);
				} else 
					printf("led blink done!\n");
				sts[i+1] = (sts[i+1] + 1) % 2;
				if (taskp->freq  & FREQ_FAST) {
					taskp->count = COUNT_FAST;
				} else if (taskp->freq & FREQ_NORMAL) {
					taskp->count = COUNT_NORMAL;
				} else if (taskp->freq & FREQ_SLOW) {
					taskp->count = COUNT_SLOW;
				}
			}
			 if (taskp->count > 0) 
				 taskp->count--;
		}
		if (taskp->time != TIME_FOREVER)
			taskp->time = taskp->time - WORKER_TIMER*1000;
		if (taskp->time <= 0 && taskp->time != TIME_FOREVER) {
			taskp->mode = MODE_NONE;
			pic_write_disk_gen(i, PIC_LED_OFF);
		}
	}
}

void worker_init(void)
{
	led_loop = EV_DEFAULT;
	
	ev_timer_init(&worker_timer, timer_cb, 0., WORKER_TIMER);
	ev_timer_again(led_loop, &worker_timer);
	ev_timer_start(led_loop, &worker_timer);

	ev_run(led_loop, 0);
}

void worker_release(void)
{
	ev_timer_stop(led_loop, &worker_timer);
	ev_break(led_loop, EVBREAK_ALL);
}
