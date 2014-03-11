#ifndef LED_WORKER__H__
#define LED_WORKER__H__

#include "common.h"
#define WORKER_TIMER .125


void led_on(led_work_args_t *data);
void worker_init(void);

#endif // LED_WORKER__H__
