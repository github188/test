#ifndef LED_WORK__H__
#define LED_WORK__H__

#include "list.h"
#include "common.h"

extern struct list _g_led_list;

void led_workqueue_init(void);
void led_workqueue_release(void);


#endif // LED_WORK__H__
