#!/usr/bin/env bash

grep 'test-main.py' /etc/rc.local > /dev/null
if [ $? -ne 0 ]; then
	echo -e "/root/pressure/test-main.py >> \$(date '+%Y%m%d-%H%M')-testlog" >> /etc/rc.local
fi

