#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import getopt
from libcase import *
from libexecutor import *

def test_case(fname):
	sys.exit(CaseExecute(CaseLoad(fname)))

def test_case_list(path):
	_case_list = []
	try:
		for x in os.listdir(path):
			if not os.path.isfile(x):
				continue
			_case_list.append(CaseLoad(x))
	except:
		print '加载测试例失败!'
		sys.exit(-1)

	for x in _case_list:
		if CaseExecute(x) != 0:
			print '运行测试例失败!'
			sys.exit(-1)
	sys.exit(0)

def usage():
	print """
test.py  --run-case </path/to/filename>
         --run-list </path/to/case/dir>
"""

opt_list = ['run-case=', 'run-list=']

def test_main():
	try:
		opts, args = getopt.gnu_getopt(sys.argv[1:], '', opt_list)
	except getopt.GetoptError, e:
		print '参数错误: ', e
		sys.exit(-1)

	for opt,arg in opts:
		if opt == '--run-case':
			test_case(arg)
		elif opt == '--run-case-list':
			test_case_list(arg)
	usage()

if __name__ == '__main__':
	test_main()
