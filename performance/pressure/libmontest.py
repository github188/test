#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import datetime
import commands
import re
import sys
from liblogtest import mon_log

reload(sys)
sys.setdefaultencoding('utf8')

stat_bin='/root/pressure/iostat'

class mon_obj:
	def __init__(self):
		self.name = ''
		self.mon_type = ''	# process, eth, speed
		self.param = ''
		self.func = None

mon_list = []

def _mon_check_process(param):
	ret,text = commands.getstatusoutput('ps -e')
	if ret != 0:
		mon_log('ERROR', '获取进程列表失败!')
		return
	xx = re.findall(param, text)
	if xx == []:
		mon_log('ERROR', '进程 %s 不存在!' % param)
	return

def _mon_check_eth(param):
	try:
		nic = re.findall('^eth\d', param)[0]
		ipaddr = re.findall('\d+.\d+.\d+.\d+', param)[0]
	except:
		mon_log('WARN', '无法解析网络参数 %s' % param)
		return
	ret,text = commands.getstatusoutput('ping -c 1 -W 1 -I %s %s' % (nic, ipaddr))
	if ret != 0:
		mon_log('ERROR', '通过网口 %s 连接IP %s 失败!' % (nic, ipaddr))
	return

def _mon_check_speed(param):
	try:
		dev = re.findall('\/dev\/md\d+', param)[0]
		r_speed = re.findall('read_speed=\d+', param)[0]
		w_speed = re.findall('write_speed=\d+', param)[0]
	except:
		mon_log('WARN', '无法解析速率检测参数 %s' % param)
		return
	ret,text = commands.getstatusoutput('%s %s' % (stat_bin, dev))
	if ret != 0:
		mon_log('WARN', '获取块设备 %s 速率失败!' % dev)
		return
	try:
		speed_str = re.findall('md\d+ +\d+.\d+ +\d+.\d+ +\d+.\d+', text)[0]
	except:
		mon_log('WARN', '无法解析块设备 %s 速率结果 %s' % (dev, text))
		return
	r_speed = float(speed_str.split()[2])
	w_speed = float(speed_str.split()[3])
	if r_speed == 0.0 and w_speed == 0.0:
		mon_log('ERROR', '块设备 %s 读写速率均为0，出现异常!' % dev)
	return

_func_map = {"process":_mon_check_process, "eth":_mon_check_eth, "speed":_mon_check_speed}

def mon_add(name, mon_type, param = ''):
	mon = mon_obj()
	mon.name = name
	mon.mon_type = mon_type
	mon.param = param
	try:
		mon.func = _func_map[mon_type]
	except:
		mon_log('WARN', '设置 %s 检查函数失败!' % name)
	mon_list.append(mon)
	mon_log('INFO', '增加 %s 成功' % name)

def do_mon(timeout = 0):
	start_time = datetime.datetime.now()
	if timeout > 0:
		mon_timeout = datetime.timedelta(hours=timeout)
		mon_log('INFO', '设置监控超时时间 %d 小时' % timeout)
	else:
		mon_log('INFO', '未设置监控超时，程序持续运行!')
	while True:
		time.sleep(60)
		for m in mon_list:
			if m.func != None:
				#mon_log('检查 %s 开始' % m.name)
				m.func(m.param)
				#mon_log('检查 %s 结束' % m.name)
				continue
			mon_log('WARN', '检查 %s 无法启动，检查函数不存在')
		if timeout <= 0:
			continue
		curr_time = datetime.datetime.now()
		if curr_time - start_time > mon_timeout:
			mon_log('WARN', '监控进程达到超时时间退出!')
			break
	return
