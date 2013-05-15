#!/usr/bin/env bash

grep 'test-main.py' /etc/rc.local > /dev/null
if [ $? -ne 0 ]; then
	echo -e "export PATH=/root/pressure:$PATH" >> /etc/rc.local
	echo -e "/root/pressure/test-main.py 2>&1 > /root/\$(date '+%Y%m%d-%H%M')-testlog &" >> /etc/rc.local
fi

