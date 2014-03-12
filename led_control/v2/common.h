#ifndef COMMON__H__
#define COMMON__H__

#define SHMKEY		"led_control"
#define SHMSIZE		16

#define DISK_ID_NONE	-1
#define DISK_ID_ALL	-2

#define MODE_NONE       0
#define MODE_ON		1
#define MODE_OFF	2
#define MODE_BLINK	4

#define FREQ_NONE	0
#define FREQ_FAST	1
#define FREQ_NORMAL	2
#define FREQ_SLOW	4

#define COUNT_FAST	1
#define COUNT_NORMAL	4
#define COUNT_SLOW	8

#define TIME_FOREVER	-200

#define VERSION	"0.0.1"
#define MAGIC   0x01234567

typedef struct led_task led_task_t;
struct led_task {
	int mode;
	long time;
	int freq;
	int count;
};
typedef struct shm_head shm_head_t;
struct shm_head {
	char version[8];
	unsigned int magic;
	int disk_num;
};
#endif // COMMON__H__
