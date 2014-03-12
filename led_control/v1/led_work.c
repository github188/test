#include "list.h"
#include "led_work.h"

struct list _g_led_list;

void led_workqueue_init(void)
{
	list_init(&_g_led_list);
	return;
}

void led_workqueue_release(void)
{
	struct list *n, *nt;
	led_work_t *work;

	list_iterate_safe(n, nt, &_g_led_list) {
		work = list_struct_base(n, led_work_t, entry);
		list_del(&work->entry);
		free(work);
	}
}

