#!/usr/bin/env sh

mddev=""
timelimit=1800
xdd_bin="/root/pressure/xdd"

[ -z "$1" ] && echo "usage $@ <mddev> [timelimit]" && exit 1
mddev="$1"
[ ! -z "$2" ] && timelimit="$2"

while :;
do
	# 顺序写半个小时
	#$xdd_bin -op write -targets 1 $mddev -dio -datapattern sequenced -reqsize 500000 -mbytes 16000000 -passes 1 -verbose -timelimit $timelimit

	# 顺序读半个小时
	#$xdd_bin -op read -targets 1 $mddev -dio -datapattern sequenced -reqsize 500000 -mbytes 16000000 -passes 1 -verbose -timelimit $timelimit > /dev/null 2>&1

	# 随机读写
	$xdd_bin  -rwratio 50 -targets 1 $mddev -dio -datapattern random -seek random -reqsize 100 -mbytes 1600000 -passes 1 -verbose -timelimit $timelimit >/dev/null 2>&1
done
