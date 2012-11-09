#!/bin/bash

fault_clean()
{
	cat /sys/block/$pdisk/make-it-fail | while read line;do
		echo $line | sed "s:.$:2:" > /sys/block/$pdisk/make-it-fail
	done
}

disk_check()
{
	local metadata_offset
	local mapped_cnt
	
	metadata_offset=`cat /sys/block/$vdisk/bad_sect_map/metadata_offset`
	if [ $metadata_offset -eq 0 ]; then
		# 启用重定向
		echo "enable" > /sys/block/$vdisk/bad_sect_map/stat
		metadata_offset=`cat /sys/block/$vdisk/bad_sect_map/metadata_offset`
		if [ $metadata_offset -eq 0 ]; then
			echo "error：重定向启用失败"
			return 1
		fi
	else
		mapped_cnt=`cat /sys/block/$vdisk/bad_sect_map/mapped_cnt`
		if [ $mapped_cnt -gt 0 ]; then
			echo "error：请清除已存在的重定向信息"
			return 1
		fi
	fi
	
	# 挂载debugfs
	if ! mount -l | grep "debugfs" >/dev/null; then
		mount -t debugfs none /sys/kernel/debug
	fi
	
	if ! mount -l | grep "debugfs" >/dev/null; then
		echo "error：debugfs挂载失败"
		return 1
	fi
	echo 100 > /sys/kernel/debug/fail_make_request/probability
	echo -1 > /sys/kernel/debug/fail_make_request/times
	
	# 清除注入的错误
	fault_clean
	
	return 0
}
