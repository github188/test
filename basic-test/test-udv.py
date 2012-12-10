#!/usr/bin/env python
# -*- coding: utf-8 -*-

import commands
import os
import json
import time

def __run_cmd(cmd):
	ret,msg = commands.getstatusoutput(cmd)
	if ret != 0:
		print 'run command: %s' % cmd
		print 'msg: %s' % msg
		return False
	retd = json.loads(msg)
	if not retd['status']:
		print 'commands: %s' % cmd
		print 'return: False'
		print 'msg: %s' % retd['msg']
		return False
	return True

def __clean_all_udv():
	ret,msg = commands.getstatusoutput('sys-manager udv --list')
	if ret == 0:
		print 'got udv'
		xx = json.loads(msg)
		for x in xx['rows']:
			print 'remove %s' % x['name']
			__run_cmd('sys-manager udv --delete %s' % x['name'])
	return

def __create_udv():
	for no in range(1,10):
		_cmd = 'sys-manager udv --create --vg slash --name udv%d --capacity 1000000' % no
		if not __run_cmd(_cmd):
			return False
		print 'create udv: udv%d' % no
		time.sleep(2)
	return True

def __remove_udv():
	for no in range(1,10):
		_cmd = 'sys-manager udv --delete udv%d' % no
		if not __run_cmd(_cmd):
			return False
		print 'remove udv: udv%d' % no
		time.sleep(2)
	return True

def udv_test():
	__clean_all_udv()
	while True:
		if not __create_udv():
			break
		if not __remove_udv():
			break
		os.popen('/usr/local/bin/md-assume.sh')
		time.sleep(3)

if __name__ == '__main__':
	udv_test()
