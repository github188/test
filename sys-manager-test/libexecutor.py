#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import commands
import json
from libcase import *

def __result_check_format(txt):
	try:
		xx = json.loads(txt)
	except:
		return False
	return True

def __case_cmd_run(cmd):
	print '%s (%s): %s' % (time.asctime(), cmd.idid, cmd.text)
	try:
		ret,msg = commands.getstatusoutput(cmd.text)
		if ret is 0:
			if __result_check_format(msg):
				print '%s OK!' % time.asctime()
			else:
				print '%s Fail: format error! %s' % (time.asctime(), msg)
		else:
			print '%s Fail: %s' % (time.asctime(), msg)
			return False
	except:
		print '%s Fail: Unknown error!' % time.asctime()
		return False
	return True

def __case_find_cmd(idid, cmd_list):
	for x in cmd_list:
		if x.idid == idid:
			return x
	return None

def CaseExecute(case = Case()):
	print '---------- 运行测试例: %s ------------' % case.name
	for x in case.workflow.cmd_id_list:
		_cmd = __case_find_cmd(x, case.cmd_list)
		if _cmd == None:
			print '找不到命令ID: ', x
			continue
		if not __case_cmd_run(_cmd):
			print '程序退出!'
			return -1
		time.sleep(1)
	return 0
