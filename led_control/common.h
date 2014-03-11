#ifndef COMMON__H__
#define COMMON__H__
#include "list.h"

#define MODE_ON		0
#define MODE_OFF	1
#define MODE_BLINK	2

#define FREQ_FAST	0
#define FREQ_NORMAL	1
#define FREQ_SLOW	2

#define COUNT_FAST	1
#define COUNT_NORMAL	4
#define COUNT_SLOW	8

#define TIME_DEFAULT 10000
/* 工作的参数 */
typedef struct led_work_args led_work_args_t;
struct led_work_args {
	int disk_id;		/* 硬盘id 如果为0点亮所有*/
	int mode;		/* 模式: 1: 点亮 2: 熄灭 3: 闪烁 */
        long time;		/* 控制的时间 */
	int freq;		/* 闪烁的频率:fast normal slow*/
	int count;		/* 计数器 */
};

/* 工作队列中的结构 */
typedef struct led_work_struct led_work_t;
struct led_work_struct {
	struct list entry;	
	void (*func)(led_work_args_t *);
	led_work_args_t data;
};

#endif // COMMON__H__
