#!/bin/bash
. ./vsd_common.sh

fault_inject()
{
	local min=$1
	local max=$2
	local cnt=$3
	local rw=$4
	
	if [ $cnt -gt $max ]; then
		cnt=$max
	fi
	
	if [ "$rw" = "r" ]; then
		rw="0"
	elif [ "$rw" = "w" ]; then
		rw="1"
	else
		rw=""
	fi
	
	list=`echo "" | awk 'BEGIN{srand()} {for (i=1;i<=cnt;i++) printf "%d\n",rand()*2000000000%max}' cnt=$cnt max=$max`

	for num in $list
	do
		if [ $num -lt $min ]; then
			continue
		fi
		if [ -z "$rw" ]; then
			if let $num%2; then
				rw="1"
			else
				rw="0"
			fi
		fi
		echo "$num $rw" > /sys/block/$pdisk/make-it-fail
	done
}

############### main start ###############
if [ -z "$1" ]; then
	echo "error：请输入要测试的磁盘，例如：sda"
	exit 1
fi

if ! ls /sys/block | grep "$1" >/dev/null 2>&1; then
	echo "error：$1 不存在"
	exit 1
fi

vdisk=$1
pdisk=p`echo $1 | cut -d 's' -f 2`
disk_test_size=200
let max_disk_sect=$disk_test_size*1024*2

# 检查磁盘
disk_check
if [ $? -ne 0 ]; then
	exit 1
fi

md=`ls /sys/block/$vdisk/holders 2>/dev/null`
if [ -z $md ]; then
	echo "error: 磁盘 $vdisk 不是raid盘"
	exit 1
fi

data_offset=`mdadm -E /dev/$vdisk 2>/dev/null | grep "Data Offset" | awk -F ':' '{ print $2 }' | awk '{ print $1 }'`
raid_disks=`cat /sys/block/$md/md/raid_disks`

#--------------------------- 随机错误，最大错误数测试
if [ ! -z $2 ]; then
	max_map_cnt=$2
else
	max_map_cnt=`cat /sys/block/$vdisk/bad_sect_map/max_map_cnt`
fi
if [ $max_map_cnt -gt $max_disk_sect ]; then
	let max_disk_sect=$max_map_cnt*2
	let disk_test_size=$max_disk_sect/2/1024+1
fi

let md_test_size=($raid_disks-1)*$disk_test_size
op_test -test /dev/$md -full_write_test -op_off 0 -test_size ${md_test_size}M >/dev/null &
pid_op_test=`ps -ef | grep "op_test -test /dev/$md" | grep -v grep | awk '{ print $1 }'`

echo "一次注入500个写错误，一直到最大错误数"

fail_total=`cat /sys/block/$pdisk/fail_total`
let fail_cnt=$max_map_cnt-$fail_total
while true
do
	if [ $fail_cnt -gt 500 ]; then
		let fail_cnt=500
	fi

	sleep 5	
	if ! ps -ef | grep "op_test" | grep -v "grep" | grep "$pid_op_test" >/dev/null; then
		echo "error：重定向未达到最大数时，op_test出错"
		exit 1
	fi
	
	fault_inject $data_offset $max_disk_sect $fail_cnt w
	fail_total=`cat /sys/block/$pdisk/fail_total`
	mapped_cnt=`cat /sys/block/$vdisk/bad_sect_map/mapped_cnt`
	echo "注入错误数量：$fail_total		已重定向数量：$mapped_cnt"
	if [ $fail_total -lt $max_map_cnt ]; then
		let fail_cnt=$max_map_cnt-$fail_total
		continue
	fi
	break
done

