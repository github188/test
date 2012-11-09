#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <arpa/inet.h>

/* 自定义netlink协议类型 */
#define NETLINK_VSD 31

/* 预警级别: 1级最高，数越大，级别越低 */
#define WARNING_LEVEL_1 1
#define WARNING_LEVEL_2 2
#define WARNING_LEVEL_3 3

struct vsd_warning_info {
	char disk_name[8];
	unsigned int mapped_cnt;
	unsigned int max_map_cnt;
};

static struct sockaddr_nl src_addr, dest_addr;

int nl_open(void)
{
	int nl_fd, res;

	nl_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_VSD);
	if (nl_fd == -1) {
		printf("%s %s\n", __FUNCTION__, strerror(errno));
		return -1;
	}

	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid();
	src_addr.nl_groups = 0; /* not in mcast groups */

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0; /* kernel */
	dest_addr.nl_groups = 0; /* unicast */

	return nl_fd;
}

static int nl_write(int fd, void *data, int len)
{
	struct iovec iov[2];
	struct msghdr msg;
	struct nlmsghdr nlh = {0};

	iov[0].iov_base = &nlh;
	iov[0].iov_len = NLMSG_HDRLEN;
	iov[1].iov_base = data;
	iov[1].iov_len = NLMSG_SPACE(len) - NLMSG_HDRLEN;

	nlh.nlmsg_len = NLMSG_SPACE(len);
	nlh.nlmsg_pid = getpid();
	nlh.nlmsg_flags = 0;
	nlh.nlmsg_type = 0;

	memset(&msg, 0, sizeof(msg));
	msg.msg_name= (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;

	return sendmsg(fd, &msg, 0);
}

static int nl_read(int fd, void *data, int len, int wait)
{
	struct iovec iov[2];
	struct msghdr msg;
	struct nlmsghdr nlh;
	int res;

	iov[0].iov_base = &nlh;
	iov[0].iov_len = NLMSG_HDRLEN;
	iov[1].iov_base = data;
	iov[1].iov_len = NLMSG_ALIGN(len);

	memset(&msg, 0, sizeof(msg));
	msg.msg_name = (void *)&src_addr;
	msg.msg_namelen = sizeof(src_addr);
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;

	res = recvmsg(fd, &msg, wait ? 0 : MSG_DONTWAIT);
	if (res > 0) {
		res -= NLMSG_HDRLEN;
		if (res < 0)
			res = -EPIPE;
		else if (res < iov[1].iov_len)
			printf("read netlink fd (%d) error: received %d"
				  " bytes but expected %zd bytes (%d)", fd, res,
				  iov[1].iov_len, len);
	}

	return res;
}

int main(int argc, char **argv)
{
	int nl_fd, warning_level, rc;
	struct vsd_warning_info warning_info;
	
	if (argc != 2) {
		printf("input warning_level!\n");
		return -1;
	}
	
	warning_level = atoi(argv[1]);
	if ((nl_fd = nl_open()) < 0) {
		perror("netlink open failed");
		return -1;
	}

	if (nl_write(nl_fd, &warning_level, sizeof(warning_level)) < 0) {
		printf("set warning level error\n");
		return -1;
	}
	
	while (1) {
		if (nl_read(nl_fd, &warning_info, sizeof(warning_info), 1) < 0) {
			if ((errno == EINTR) || (errno == EAGAIN))
				continue;
			printf("read netlink fd (%d) failed: %s", nl_fd, strerror(errno));
			return -1;
		}
		
		printf("disk warning: time %d, disk %s mapped_cnt: %u, max_map_cnt: %u\n",
				time(0), warning_info.disk_name, warning_info.mapped_cnt,
				warning_info.max_map_cnt);
	}
	
	return 0;
}
