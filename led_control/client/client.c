#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include "../common.h"

#define  PORT  9001
#define  IP  "127.0.0.1"

int expire;
char *l_opt_arg;
char *const short_options = "i:t:d:h";
led_work_args_t data;

struct option long_options[] = {
	{"table", 0, NULL, 't'},
	{"id", 1, NULL, 'i'},
	{"diskled", 1, NULL, 'd'},
	{"freq", 1, NULL, 0},
	{"expire", 1,&expire , 1},
	{"help", 1, NULL, 'h'},
	{0, 0, 0, 0}
};

void print_help(void)
{
	printf("tools-test-led-no-mcu:\n");
	printf("\t[--id|-i <disk_id>]\n");
	printf("\t[--diskled|-d on|off|[blink--freq fast|normal|slow]]\n");
	printf("\t[--expire <seconds>]\n");
	printf("\t[--table|-t]\n");
}
int my_getopt(int argc, char **argv)
{
	int c;

	while ((c = getopt_long(argc, (char *const*)argv, short_options,
				long_options, NULL)) != -1) {
		switch (c) {
		case 't':
			;
			break;
		case 'i':
			data.disk_id = atoi(optarg);
			break;
		case 'd':
			if (!strcmp(optarg, "on")) {
				data.mode = MODE_ON;
				break;
			}
			if (!strcmp(optarg, "off")) {
				data.mode = MODE_OFF;
				break;
			}
			if (!strcmp(optarg, "blink")) {
				data.mode = MODE_BLINK;
				break;
			}
			fprintf(stderr, "diskled invalid.\n");
			print_help();
			return -1;
		case 'h':
			print_help();
			return -1;
		case 0:
			if (expire) {
				data.time = atol(optarg) * 1000;
				break;
			} else {
				if (!strcmp(optarg, "fast")) {
					data.freq = FREQ_FAST;
					break;
				}
				if (!strcmp(optarg, "normal")) {
					data.freq = FREQ_NORMAL;
					break;
				}
				if (!strcmp(optarg, "slow")) {
					data.freq = FREQ_SLOW;
					break;
				}
			}
			print_help();
			return -1;
		default:
			break;
		}
	}
	return 0;
}
int main(int argc, char *argv[])
{
	if (argc < 3) {
		print_help();
		return -1;
	}
	data.freq = -1;		
	if (my_getopt(argc, argv))
		return -1;
	if (data.mode == MODE_BLINK) {
		if (data.freq != FREQ_FAST && data.freq != FREQ_NORMAL
		    && data.freq != FREQ_SLOW) {
			fprintf(stderr, "blink need freq.\n");
			return -1;
		}
	}
	printf("disk_id:%d, mode:%d, time:%ld, freq:%d, count:%d\n",
	       data.disk_id, data.mode, data.time, data.freq, data.count);
	
	int sock_fd;
	struct sockaddr_in s_addr;
	socklen_t addr_len;
	int len;
	struct msghdr msg;
	struct iovec iov[1];
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0) {
		fprintf(stderr, "create socket failed.\n");
		return -1;
	}
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
	inet_pton(AF_INET, IP, &s_addr.sin_addr);
	addr_len = sizeof(s_addr);


	sendto(sock_fd, &data, sizeof(data), 0,
	       (struct sockaddr *)&s_addr, addr_len);
	close(sock_fd);
	return 0;
}


