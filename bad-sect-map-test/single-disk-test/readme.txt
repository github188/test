预置条件

配置内核可以注入io错误
CONFIG_DEBUG_FS=y
CONFIG_DEBUG_KERNEL=y
CONFIG_FAULT_INJECTION=y
CONFIG_FAIL_MAKE_REQUEST=y
CONFIG_FAULT_INJECTION_DEBUG_FS=y

检查磁盘是否已经存在重定向
	mapped_cnt=`cat /sys/block/sda/bad_sect_map/mapped_cnt`
如果mapped_cnt大于0，清除重定向
	metadata_offset=`cat /sys/block/sda/bad_sect_map/metadata_offset`
	dd if=/dev/zero of=/dev/pda bs=512 seek=$metadata_offset count=8 oflag=direct
重新插入磁盘，或者重启系统
