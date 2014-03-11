#include <stdio.h>
#include <ev.h>
#include "led_worker.h"
#include "led_work.h"
#include "pic_ctl/pic_ctl.h"


extern struct list _g_led_list;
static ev_timer worker_timer;
extern struct ev_loop *led_loop;
extern int sts[];
static int i;
void work_release(led_work_t *work)
{
	list_del(&work->entry);
	free(work);
	return;
}

void led_on(led_work_args_t *data)
{
	if (data->time <= 0) {
		work_release(list_struct_base(data, led_work_t, data));
		return;
	}
	
	if (data->mode ==  MODE_ON) {
		//点亮
		if (pic_write_disk(data->disk_id, PIC_LED_ON) != 0) {
			 fprintf(stderr, "led on disk %d status failed.",
			 data->disk_id); 
		}
		printf("led on have done!\n");
	} else if (data->mode == MODE_OFF) {
		//熄灭
		if (pic_write_disk(data->disk_id, PIC_LED_OFF) != 0) {
			 fprintf(stderr, "led off disk %d failed.\n", 
			 data->disk_id); 
		}
		printf("led off have done!\n");
	} else if (data->mode == MODE_BLINK){
		if (data->count > 0) {
			data->count--;
		} else if (data->count == 0) {
			if (pic_read_disk(data->disk_id, &i) != 0) {
				 fprintf(stderr, "read disk %d failed.\n", 
				 data->disk_id); 
			}
			if (pic_write_disk(data->disk_id, ~i) != 0) {
				 fprintf(stderr, "write disk %d failed.\n",
				 data->disk_id); 
			}
			
			if (data->freq == FREQ_FAST) {
				data->count = COUNT_FAST;
			} else if (data->freq == FREQ_NORMAL) {
				data->count = COUNT_NORMAL;
			} else if (data->freq == FREQ_SLOW) {
				data->count = COUNT_SLOW;
			}
		}
	}
	data->time = data->time - WORKER_TIMER*1000;
	if (data->time <=0 ) {
		if (pic_write_disk(data->disk_id, sts[data->disk_id]) != 0) {
			fprintf(stderr, "restore origin status failed.\n");
		}
		work_release(list_struct_base(data, led_work_t, data));
	}
	return;
}
static void do_work(EV_P_ ev_timer *w, int r)
{
	struct list *n, *nt;
	led_work_t *work;

	if (list_empty(&_g_led_list)) {
		printf("list empyt\n");
		return ;
	}
	list_iterate_safe(n, nt, &_g_led_list) {
		work = list_struct_base(n, led_work_t, entry);
		work->func(&work->data);
	}
}

void worker_init(void)
{
	ev_timer_init(&worker_timer, do_work, 0., WORKER_TIMER);
        ev_timer_again(led_loop, &worker_timer);
	ev_timer_start(led_loop, &worker_timer);
}

void worker_release(void)
{
	
}
