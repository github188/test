#include <stdio.h>
#include <ev.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#include "led_ev.h"
#include "common.h"
#include "led_worker.h"
#include "led_work.h"
#include "pic_ctl/pic_ctl.h"



struct ev_loop *led_loop = NULL;
ev_io led_ev_io;
extern struct list _g_led_list;
int sts[PIC_LED_NUMBER];

typedef struct led_ev led_ev_t;
struct led_ev {
	ev_io ev_io;
	led_work_args_t data;
};

static void led_io_cb(EV_P_ ev_io *w, int r)
{

	led_work_t *work;
	led_work_args_t data;
	
	memset(&data, 0, sizeof(data));
	
	printf("catch!\n");
	work = (led_work_t *)malloc(sizeof(led_work_t));
	if (work == NULL) {
		fprintf(stderr, "malloc failed.");
		return;
	}


	recv(w->fd, &data, sizeof(data), 0);

	work->data.disk_id = data.disk_id;
	if (work->data.disk_id > PIC_LED_NUMBER) {
		fprintf(stderr, "disk_id %d invalid.\n", work->data.disk_id);
		return;
	}
	
	work->data.time = data.time;
	if (work->data.time <= 0) {
		fprintf(stderr, "time is invalid, use default time.");
		work->data.time = TIME_DEFAULT;
		
	}
	if (pic_read_disk(work->data.disk_id, &sts[work->data.disk_id]) != 0) {
		fprintf(stderr, "get origin status failed.\n");
	}
	work->data.mode = data.mode;
	if (data.mode == MODE_BLINK) {
		work->data.freq = data.freq;
		if (data.freq == FREQ_FAST) {
			work->data.count = COUNT_FAST;
		} else if (data.freq == FREQ_NORMAL) {
			work->data.count = COUNT_NORMAL;
		} else if (data.freq == FREQ_SLOW) {
			work->data.count = COUNT_SLOW;
		} else {
			fprintf(stderr, "invalid freq.");
			free(work);
			return ;
		}
	}

	work->func = led_on;
	printf("recv disk_id:%d, mode:%d, time:%ld, freq:%d count:%d\n",
	       work->data.disk_id, work->data.mode, work->data.time,
	       work->data.freq, work->data.count);
	list_add(&_g_led_list, &work->entry);
	printf("added\n");
	return;
}

static int init_sock(int port)
{
	int sock_fd;
	struct sockaddr_in s_addr;

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0) {
		fprintf(stderr, "%s socket create failed: %s\n",
			__func__, strerror(errno));
		return -1;
	}

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	inet_pton(AF_INET, IP, &s_addr.sin_addr);

	if (bind(sock_fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) {
		fprintf(stderr, "%s bind failed: %s\n", __func__,
			strerror(errno));
		return -1;
	}

	return sock_fd;
}

void led_ev_init(void)
{
	int fd = init_sock(PORT);
	if (fd < 0)
		return ;
	
	led_loop = EV_DEFAULT;

	ev_io_init(&led_ev_io, led_io_cb, fd, EV_READ);
	ev_io_start(led_loop, &led_ev_io);


}

void led_ev_release(void)
{
	ev_io_stop(led_loop, &led_ev_io);
	close(led_ev_io.fd);
}
