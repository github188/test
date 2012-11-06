#!/usr/bin/env sh

mddev=""
timelimit=1800
result_dir="/root/xdd-result"

[ -z "$1" ] && echo "usage $@ <mddev> [timelimit]" && exit 1
mddev="$1"
[ ! -z "$2" ] && timelimit="$2"

mkdir -p $result_dir

while :;
do
	# 顺序写半个小时
	tmp="$result_dir/xdd-write-$(date +%Y%m%d-%H%M%S)"
	xdd -op write -targets 1 $mddev -dio -datapattern random -reqsize 500000 -mbytes 16000000 -passes 1 -verbose -timelimit $timelimit > "$tmp" 2>&1

	# 顺序读半个小时
	tmp="$result_dir/xdd-read-$(date +%Y%m%d-%H%M%S)"
	xdd -op read -targets 1 $mddev -dio -datapattern random -reqsize 500000 -mbytes 16000000 -passes 1 -verbose -timelimit $timelimit > "$tmp" 2>&1

	# 随机读写
	tmp="$result_dir/xdd-random-$(date +%Y%m%d-%H%M%S)"
	xdd  -rwratio 50 -targets 1 $mddev -dio -datapattern random -seek random -reqsize 100 -mbytes 1600000 -passes 1 -verbose -timelimit $timelimit "$tmp" 2>&1
done
