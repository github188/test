#!/bin/bash -x

. ./vsd_common.sh

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
rdisk=r`echo $1 | cut -d 's' -f 2`

metadata_offset=`cat /sys/block/$vdisk/bad_sect_map/metadata_offset`
dd if=/dev/zero of=/dev/pd${vdisk##sd} bs=512 seek=$metadata_offset count=8 oflag=direct
