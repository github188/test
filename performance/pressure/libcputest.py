#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from libmontest import mon_add

_cpu_exec = '/root/pressure/kill-idle.sh'

def do_cpu_test():
	os.popen('%s &' % _cpu_exec)
	os.popen('%s &' % _cpu_exec)
	os.popen('%s &' % _cpu_exec)
	os.popen('%s &' % _cpu_exec)
	mon_add('CPU压力测试', 'process', 'yes')
