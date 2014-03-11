#include <stdio.h>
#include <ev.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
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

typedef struct led_ev led_ev_t;
struct led_ev {
	ev_io ev_io;
	led_work_args_t data;
};

static void led_io_cb(EV_P_ ev_io *w, int r)
{

	led_work_t *work, *tmp;
	led_work_args_t data;
	struct list *n, *nt;
	
	memset(&data, 0, sizeof(data));
	
	printf("catch!\n");
	work = (led_work_t *)malloc(sizeof(led_work_t));
	if (work == NULL) {
		fprintf(stderr, "malloc failed.");
		return;
	}

	recv(w->fd, &data, sizeof(data), 0);
	work->data.disk_id = data.disk_id - 1;
	
	list_iterate_safe(n, nt, &_g_led_list) {
		tmp = list_struct_base(n, led_work_t, entry);
		if (tmp->data.disk_id == work->data.disk_id) {
			work_release(tmp);
		}
	}
	if (work->data.disk_id >= PIC_LED_NUMBER) {
		fprintf(stderr, "disk_id %d invalid.\n", work->data.disk_id);
		return;
	}
	
	work->data.time = data.time;
	if (work->data.time <= 0) {
		work->data.time = TIME_FOREVER;
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

static int init_sock(void)
{
	int sock_fd;
	struct sockaddr_un s_addr;

	sock_fd = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (sock_fd < 0) {
		fprintf(stderr, "%s socket create failed: %s\n",
			__func__, strerror(errno));
		return -1;
	}

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sun_family = AF_UNIX;
	strncpy(s_addr.sun_path, UNIX_DOMAIN, sizeof(s_addr.sun_path)-1);
	unlink(UNIX_DOMAIN);

	if (bind(sock_fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) {
		fprintf(stderr, "%s bind failed: %s\n", __func__,
			strerror(errno));
		return -1;
	}

	return sock_fd;
}

void led_ev_init(void)
{
	int fd = init_sock();
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
