#!/bin/bash
. ./vsd_common.sh

verify_map()
{
	mapped_cnt=`cat /sys/block/$vdisk/bad_sect_map/mapped_cnt`
	if [ $mapped_cnt -ne 0 ]; then
		echo "error：重定向个数不为0"
		return 1
	fi
	
	mapped_sectors=`cat /sys/block/$vdisk/bad_sect_map/mapped_sectors`
	if [ ! -z "$mapped_sectors" ]; then
		echo "error：重定向map不为空"
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

# 检查磁盘
disk_check
if [ $? -ne 0 ]; then
	exit 1
fi

echo "super扇区测试，注入写错误"
metadata_offset=`cat /sys/block/$vdisk/bad_sect_map/metadata_offset`
let sect=$metadata_offset+2
echo "${sect} 1" > /sys/block/$pdisk/make-it-fail

echo "0扇区注入读错误"
echo "0 0" > /sys/block/$pdisk/make-it-fail
echo "读0扇区"
dd if=/dev/$vdisk of=/dev/null bs=512 count=1 iflag=direct
if [ $? -eq 0 ]; then
	echo "error：dd读错误扇区没有出错"
	exit 1
fi

verify_map
if [ $? -ne 0 ]; then
	echo "error：验证重定向失败"
	exit 1
fi

echo "0扇区注入写错误"
echo "0 1" > /sys/block/$pdisk/make-it-fail
echo "写0扇区"
dd of=/dev/$vdisk if=/dev/zero bs=512 count=1 oflag=direct
if [ $? -eq 0 ]; then
	echo "error：dd写错误扇区没有出错"
	exit 1
fi

verify_map
if [ $? -ne 0 ]; then
	echo "error：验证重定向失败"
	exit 1
fi

echo "super扇区测试成功"

echo "清除super扇区错误"
echo "${sect} 3" > /sys/block/$pdisk/make-it-fail

echo "map扇区测试，注入写错误"
metadata_offset=`cat /sys/block/$vdisk/bad_sect_map/metadata_offset`
let sect=$metadata_offset+8
echo "${sect} 1" > /sys/block/$pdisk/make-it-fail

echo "0扇区注入读错误"
echo "0 0" > /sys/block/$pdisk/make-it-fail
echo "读0扇区"
dd if=/dev/$vdisk of=/dev/null bs=512 count=1 iflag=direct
if [ $? -eq 0 ]; then
	echo "error：dd读错误扇区没有出错"
	exit 1
fi

verify_map
if [ $? -ne 0 ]; then
	echo "error：验证重定向失败"
fi

echo "0扇区注入写错误"
echo "0 1" > /sys/block/$pdisk/make-it-fail
echo "写0扇区"
dd of=/dev/$vdisk if=/dev/zero bs=512 count=1 oflag=direct
if [ $? -eq 0 ]; then
	echo "error：dd写错误扇区没有出错"
	exit 1
fi

verify_map
if [ $? -ne 0 ]; then
	echo "error：验证重定向失败"
	exit 1
fi

echo "map扇区测试成功"

echo "清除map扇区错误"
echo "${sect} 3" > /sys/block/$pdisk/make-it-fail

echo "data扇区测试，注入写错误"
sect=`cat /sys/block/$vdisk/bad_sect_map/data_offset`
echo "${sect} 1" > /sys/block/$pdisk/make-it-fail

echo "0扇区注入读错误"
echo "0 0" > /sys/block/$pdisk/make-it-fail
echo "读0扇区"
dd if=/dev/$vdisk of=/dev/null bs=512 count=1 iflag=direct
if [ $? -eq 0 ]; then
	echo "error：dd读错误扇区没有出错"
	exit 1
fi

mapped_cnt=`cat /sys/block/$vdisk/bad_sect_map/mapped_cnt`
if [ $mapped_cnt -ne 1 ]; then
	echo "error：重定向个数不为1"
	return 1
fi

mapped_sectors=`cat /sys/block/$vdisk/bad_sect_map/mapped_sectors | grep "0 -> $sect"`
if [ -z "$mapped_sectors" ]; then
	echo "error：重定向map为空"
	return 1
fi

echo "${sect} 3" > /sys/block/$pdisk/make-it-fail
echo "data扇区测试，注入写错误"
let sect=$sect+1
echo "${sect} 1" > /sys/block/$pdisk/make-it-fail

echo "1扇区注入写错误"
echo "1 1" > /sys/block/$pdisk/make-it-fail
echo "写1扇区"
dd of=/dev/$vdisk if=/dev/zero bs=512 count=1 seek=1 oflag=direct
if [ $? -eq 0 ]; then
	echo "error：dd写错误扇区没有出错"
	exit 1
fi

mapped_cnt=`cat /sys/block/$vdisk/bad_sect_map/mapped_cnt`
if [ $mapped_cnt -ne 2 ]; then
	echo "error：重定向个数不为1"
	return 1
fi

mapped_sectors=`cat /sys/block/$vdisk/bad_sect_map/mapped_sectors | grep "1 -> $sect"`
if [ -z "$mapped_sectors" ]; then
	echo "error：重定向map为空"
	return 1
fi

echo "再写一遍，将直接重定向写data扇区"
dd of=/dev/$vdisk if=/dev/zero bs=512 count=1 seek=1 oflag=direct
if [ $? -eq 0 ]; then
	echo "error：dd写错误扇区没有出错"
	exit 1
fi

echo "data扇区测试成功"

echo "${sect} 3" > /sys/block/$pdisk/make-it-fail
echo "0 3" > /sys/block/$pdisk/make-it-fail
