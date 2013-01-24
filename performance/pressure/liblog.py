#!/usr/bin/env python
# -*- coding: utf-8 -*-

import datetime
import sys

reload(sys)
sys.setdefaultencoding('utf8')

def mon_log(log_type, msg):
	print datetime.datetime.now(), log_type, msg

if __name__ == '__main__':
	mon_log('INFO', '测试日志信息')
