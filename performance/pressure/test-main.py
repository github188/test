#!/usr/bin/python -u
# -*- coding: utf-8 -*-

import os
from libmdtest import do_md_test
from libnetworktest import do_network_test
from libcputest import do_cpu_test
from libmontest import do_mon
from liblogtest import mon_log

def test_main():
	mon_log('INFO', '启动测试进程')
	os.popen('2>&1 /root/pressure/cleanup.sh >/dev/null')
	do_md_test()
	do_network_test()
	do_cpu_test()
	timeout = 0
	try:
		if len(sys.argv) == 2:
			timeout = int(sys.argv[1])
	except:
		pass
	do_mon(timeout)

if __name__ == '__main__':
	test_main()
