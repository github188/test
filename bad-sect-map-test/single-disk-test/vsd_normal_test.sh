#!/bin/bash
. ./vsd_common.sh

fault_inject()
{
	local max=$1
	local cnt=$2
	local rw=$3
	
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

verify_sect_map()
{
	local r_map_stat=$1
	local sector
	local rw
	local map
	local map_list
	local map_cnt
	local inject_cnt
	local mapped_cnt
	
	map_list=`cat /sys/block/$vdisk/bad_sect_map/mapped_sectors`	
	while read line
	do
		sector=`echo $line | cut -d ' ' -f 1`
		rw=`echo $line | cut -d ' ' -f 2`
		
		map=`echo $map_list | grep "$sector ->"`
		if [ -z "$map" ]; then
			echo "error：sector: $sector not map"
			return 1
		fi
		
		map_stat=`echo $map | cut -d ',' -f 2 | cut -d ')' -f 1`
		
		if [ "$rw" = "1" ]; then
			if [ $map_stat -ne 2 ]; then
				echo "error：sector: $sector stat error"
				return 1
			fi
		else
			if [ $map_stat -ne $r_map_stat ]; then
				echo "error：sector: $sector stat error"
				return 1
			fi
		fi
	done < /sys/block/$pdisk/make-it-fail
	
	inject_cnt=`cat /sys/block/$pdisk/fail_total`
	mapped_cnt=`cat /sys/block/$vdisk/bad_sect_map/mapped_cnt`
	if [ $inject_cnt -ne $mapped_cnt ]; then
		echo "error：mapped_cnt: $mapped_cnt error, inject error count: $inject_cnt"
		return 1
	fi
	
	map_cnt=`cat /sys/block/$vdisk/bad_sect_map/mapped_sectors | wc -l`
	if [ $inject_cnt -ne $map_cnt ]; then
		echo "error：map list count: $map_cnt error, inject error count: $inject_cnt"
		return 1
	fi
	
	return 0
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
test_size=200
let max_sect=$test_size*1024*2

# 检查磁盘
disk_check
if [ $? -ne 0 ]; then
	exit 1
fi

#----------------------- 注入读错误
echo "读错误重定向测试，注入1个读错误"
# 注入1个读错误
fault_inject $max_sect 1 r

# 读虚拟磁盘
echo "读错误扇区"
dd of=/dev/null if=/dev/$vdisk bs=1M count=$test_size iflag=direct
if [ $? -eq 0 ]; then
	echo "error：读错误扇区测试失败：dd读错误扇区没有出错"
	exit 1
fi

# 验证重定向
verify_sect_map 0
if [ $? -ne 0 ]; then
	echo "error：验证重定向失败"
	exit 1
fi
echo "读操作测试成功"

# 写虚拟磁盘
echo "写错误扇区"
dd if=/dev/zero of=/dev/$vdisk bs=1M count=$test_size oflag=direct
if [ $? -ne 0 ]; then
	echo "error：写错误测试失败：dd写错误扇区出错"
	exit 1
fi

# 验证重定向
verify_sect_map 2
if [ $? -ne 0 ]; then
	echo "error：再次验证重定向失败"
	exit 1
fi
echo "读错误重定向测试成功"

#------------------------ 注入写错误
echo "写错误重定向测试，注入10个写错误"
# 注入10个写错误
fault_inject $max_sect 10 w

# 写虚拟磁盘
echo "写错误扇区"
dd if=/dev/zero of=/dev/$vdisk bs=1M count=$test_size oflag=direct
if [ $? -ne 0 ]; then
	echo "error：写错误测试失败：dd写错误扇区出错"
	exit 1
fi

# 验证重定向
verify_sect_map 2
if [ $? -ne 0 ]; then
	echo "error：验证重定向失败"
	exit 1
fi
echo "写错误重定向测试成功"

#--------------------------- 随机错误，最大错误数测试
echo "随机错误，最大错误数测试"
echo "op_test写测试，校验写入数据"
max_map_cnt=`cat /sys/block/$vdisk/bad_sect_map/max_map_cnt`
if [ $max_map_cnt -gt $max_sect ]; then
	let max_sect=$max_map_cnt*2
	let test_size=$max_sect/2/1024+1
fi
op_test -test /dev/$vdisk -full_write_test -op_off 0 -test_size ${test_size}M >/dev/null &
pid_op_test=`ps -ef | grep "op_test -test /dev/$vdisk" | grep -v grep | awk '{ print $1 }'`

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
	
	fault_inject $max_sect $fail_cnt w
	fail_total=`cat /sys/block/$pdisk/fail_total`
	mapped_cnt=`cat /sys/block/$vdisk/bad_sect_map/mapped_cnt`
	echo "注入错误数量：$fail_total		已重定向数量：$mapped_cnt"
	if [ $fail_total -lt $max_map_cnt ]; then
		let fail_cnt=$max_map_cnt-$fail_total
		continue
	fi
	break
done

echo "等待重定向达到最大数量"
while true
do
	mapped_cnt=`cat /sys/block/$vdisk/bad_sect_map/mapped_cnt`
	echo "已重定向数量：$mapped_cnt"
	if [ $mapped_cnt -lt $max_map_cnt ]; then
		sleep 10
	else
		break
	fi
	
	if ! ps -ef | grep "op_test" | grep -v "grep"| grep "$pid_op_test" >/dev/null; then
		echo "error：重定向未达到最大数时，op_test出错"
		exit 1
	fi
done

if ! ps -ef | grep "op_test" | grep -v "grep" | grep "$pid_op_test" >/dev/null; then
	echo "error：重定向达到最大数时，op_test出错"
	exit 1
fi
echo "重定向达到最大数，op_test运行正常，杀死op_test进程"
kill -9 $pid_op_test

echo "再注入一个错误，dd写测试"
while true
do
	fault_inject $max_sect 1 w
	fail_total=`cat /sys/block/$pdisk/fail_total`
	if [ $fail_total -le $max_map_cnt ]; then
		continue
	fi
	break
done

dd if=/dev/zero of=/dev/$vdisk bs=1M count=$test_size oflag=direct
if [ $? -eq 0 ]; then
	echo "error：dd写没有出错"
	exit 1
fi

echo "随机错误，最大错误数测试成功"

echo ""
echo "请手动重启系统，然后启动op_test测试，op_test不出错则测试成功，否则测试失败"
echo "op_test命令参数：op_test -test /dev/$vdisk -full_write_test -op_off 0 -test_size ${test_size}M"
