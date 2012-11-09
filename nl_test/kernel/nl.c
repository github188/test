#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <net/tcp.h>

/* 自定义netlink协议类型 */
#define NETLINK_VSD 31

/* 预警级别: 1级最高，数越大，级别越低 */
#define WARNING_LEVEL_1 1
#define WARNING_LEVEL_2 2
#define WARNING_LEVEL_3 3

static struct task_struct *tsk;
static struct sock *nl_sk = NULL;
static pid_t pid = 0;
static int warning_level = 2;
static int nl_error = 0;

struct vsd_warning_info {
	char disk_name[8];
	unsigned int mapped_cnt;
	unsigned int max_map_cnt;
};

static int nl_send_msg(const void *buf, int buf_len)
{
	int res = 0, len;
	struct sk_buff *skb;
	struct nlmsghdr *nlh;

	len = NLMSG_SPACE(buf_len);

	skb = alloc_skb(len, GFP_KERNEL);
	if (skb == NULL) {
		printk("[vsd] alloc_skb() failed (len %d)\n", len);
		res =  -ENOMEM;
		goto out;
	}

	nlh = __nlmsg_put(skb, pid, 0, 0, buf_len, 0);

	memcpy(NLMSG_DATA(nlh), buf, buf_len);
	res = netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT);
	if (res <= 0) {
		if (res != -ECONNREFUSED)
			printk("[vsd] netlink_unicast() failed: %d", res);
		else {
			pid = 0;
			printk("[vsd] netlink_unicast() failed: Connection refused. "
					"Not functioning user space?\n");
		}
		goto out;
	}

out:
	return res;
}

static void do_warning(const char *disk_name, unsigned int mapped_cnt,
						unsigned int max_map_cnt)
{
	struct vsd_warning_info warning_info;
	int do_warning = 0, rc;

	if (0 == pid)
		return;

	switch (warning_level) {
	case 1:
		do_warning = 1;
		break;
	case 2:
		if (mapped_cnt%10 == 0)
			do_warning = 1;
		break;
	case 3:
		if (mapped_cnt%100 == 0)
			do_warning = 1;
		break;
	}

	if (!do_warning && !nl_error)
		return;
	
	strcpy(warning_info.disk_name, disk_name);
	warning_info.mapped_cnt = mapped_cnt;
	warning_info.max_map_cnt = max_map_cnt;

	rc = nl_send_msg(&warning_info, sizeof(warning_info));
	if (rc <= 0)
		nl_error = rc;
	else
		nl_error = 0;
}

static int nl_recv_msg(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	pid = NETLINK_CB(skb).pid;
	WARN_ON(pid == 0);
	
	warning_level = *(int *)NLMSG_DATA(nlh);

	printk("[nl_test] pid: %d, warning_level: %d\n", pid, warning_level);
	return 0;
}

static void vsd_recv_skb(struct sk_buff *skb)
{
	int err;
	struct nlmsghdr	*nlh;
	u32 rlen;

	while (skb->len >= NLMSG_SPACE(0)) {
		nlh = (struct nlmsghdr *)skb->data;
		if (nlh->nlmsg_len < sizeof(*nlh) || skb->len < nlh->nlmsg_len)
			return;
		rlen = NLMSG_ALIGN(nlh->nlmsg_len);
		if (rlen > skb->len)
			rlen = skb->len;
		err = nl_recv_msg(skb, nlh);
		if (err)
			netlink_ack(skb, nlh, -err);
		else if (nlh->nlmsg_flags & NLM_F_ACK)
			netlink_ack(skb, nlh, 0);
		skb_pull(skb, rlen);
	}
}

static int warning_thread(void * arg)
{
	unsigned int mapped_cnt = 0;
	unsigned int max_map_cnt = 500000;
	while(!kthread_should_stop()) {
		do_warning("sdx", mapped_cnt++, max_map_cnt);
		msleep(1000);
	}
	return 0;
}

static int __init nl_test_init(void)
{
	nl_sk = netlink_kernel_create(&init_net, NETLINK_VSD, 1, vsd_recv_skb,
									NULL, THIS_MODULE);
	if (!nl_sk)
		return -ENOMEM;
	
	tsk = kthread_run(warning_thread, NULL, "nl_test");
	if (IS_ERR(tsk)) {
		netlink_kernel_release(nl_sk);
		return -ENOMEM;
	}
	
	return 0;
}

static __exit void nl_test_exit(void)
{
	kthread_stop(tsk);
	netlink_kernel_release(nl_sk);
}

module_init(nl_test_init);
module_exit(nl_test_exit);

MODULE_DESCRIPTION("netlink test");
MODULE_AUTHOR("zhangyf");
MODULE_LICENSE("GPL");