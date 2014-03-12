#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"
#include "led_shm.h"

extern int disk_max_num;
shm_head_t *addr;
int shm_init()
{
	int shmid;
	key_t shmkey;
	size_t size;

	
	shmkey = ftok(SHMKEY, 0);
	size = (sizeof(led_task_t) * (disk_max_num + 1) + sizeof(shm_head_t));
	shmid = shmget(shmkey, size,  0666|IPC_CREAT);
	if (shmid == -1) {
		fprintf(stderr, "create shm failed.\n");
		return -1;
	}
	addr = (shm_head_t *)shmat(shmid, 0, 0);
	if (addr == (shm_head_t*)-1) {
		fprintf(stderr, "shmat failed.\n");
		return -1;
	}
	strcpy(addr->version, VERSION);
	addr->magic = MAGIC;
	addr->disk_num = disk_max_num;

	printf("shmid : %d addr: %p\n", shmid, addr);

	return shmid;
}

void shm_release(void)
{
	int ret = -1;
	int shmid;
	key_t shmkey;

	shmkey = ftok(SHMKEY, 0);
	shmid = shmget(shmkey, 0, 0666);
	ret = shmctl(shmid, IPC_RMID, NULL);
	if (ret == -1) {
		fprintf(stderr, "release shm failed.\n");
		return;
	}
}
