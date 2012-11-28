#!/usr/bin/env python
# -*- coding: utf-8 -*-

import commands, sys
from time import asctime
from time import localtime

def __run_cmd(_cmd):
	print asctime(localtime()), 'RUN: ', _cmd
	ret,msg = commands.getstatusoutput(_cmd)
	if ret is not 0:
		print '命令出错!'
		print _cmd
		print '出错信息: ', msg
		sys.exit(-1)
	return

def vg_test():
	__run_cmd('sys-manager vg --create --name slash --level 5 --strip 128 --disks 0:3,0:4,0:5')
	udv_test()
	__run_cmd('sys-manager vg --delete slash')

def udv_test():
	for idid in range(1, 10):
		__run_cmd('sys-manager udv --create --vg slash --name udv%d --capacity 1000000' % idid)

	for idid in range(1, 10):
		__run_cmd('sys-manager udv --delete udv%d' % idid)

def test_case1():
	vg_test()

if __name__ == '__main__':
	while True:
		test_case1()
