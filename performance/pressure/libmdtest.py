#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import commands
import sys
import os
from libmontest import mon_add
from libmontest import mon_log

reload(sys)
sys.setdefaultencoding('utf8')

op_bin='/root/pressure/test-raid.sh'

def do_md_test():
	ret,text = commands.getstatusoutput('sys-manager vg --list')
	if ret != 0:
		mon_log('WARN', '无法获取卷组列表，请检查卷组配置')
		return
	try:
		md_list = json.loads(text)['rows']
		for md in md_list:
			os.popen('%s %s &' % (op_bin, md['dev']))
			mon_add('%s %s 压力测试-进程检测' % (md['name'], md['dev']), 'process', 'xdd')
			mon_add('%s %s 压力测试-速率检测' % (md['name'], md['dev']),  'speed', '%s,read_speed=1,write_speed=1' % md['dev'])
	except:
		mon_log('WARN', '解析卷组列表出错，请检查')
		sys.exit(-1)
	return
