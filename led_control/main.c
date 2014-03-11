#include <stdio.h>
#include <ev.h>
#include "common.h"
#include "led_ev.h"
#include "led_work.h"
#include "led_worker.h"


extern struct ev_loop *led_loop;

int main(int argc, char *argv[])
{

	led_workqueue_init();
	led_ev_init();
	worker_init();

	ev_run(led_loop, 0);
	return 0;
}
