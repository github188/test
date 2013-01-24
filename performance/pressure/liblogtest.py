#!/usr/bin/python -u
# -*- coding: utf-8 -*-

import datetime
import sys

reload(sys)
sys.setdefaultencoding('utf8')

def mon_log(log_type, msg):
	"""
	try:
		f = open('/root/test-log', 'rw+')
		log_msg = '%s %s %s' % (datetime.datetime.now(), log_type, msg)
		f.write(log_msg)
		f.close()
	except:
		pass
	"""
	print datetime.datetime.now(), log_type, msg

if __name__ == '__main__':
	mon_log('INFO', '测试日志信息')
