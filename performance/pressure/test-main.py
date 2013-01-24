#!/usr/bin/env python
# -*- coding: utf-8 -*-

from libmd import do_md_test
from libnetwork import do_network_test
from libcpu import do_cpu_test
from libmon import do_mon
from liblog import mon_log

def test_main():
	mon_log('INFO', '启动测试进程')
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
