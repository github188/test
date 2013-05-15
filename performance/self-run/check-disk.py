#!/usr/bin/env python
# -*- coding: utf-8 -*-

import commands, json
import sys
import time

reload(sys)
sys.setdefaultencoding('utf-8')

def check_disk():
	try:
		cmd = 'sys-manager disk --list'
		disk_list = json.loads(commands.getoutput(cmd))
		if int(disk_list['total']) != 16:
			f = open("/root/disk-check-log", "a")
			f.write("磁盘数量小于16个!");
			f.write(str(disk_list))
			f.close()
	except:
		pass

if __name__ == '__main__':
	while True:
		check_disk()
		time.sleep(300)
