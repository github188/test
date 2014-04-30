#!/bin/bash

version()
{
	echo "hw-test - v0.1 - 20140430 12:00"
}

help() {
	echo "usage:"
	echo "	hw-test [date][cpu][mem][sysdisk][network][disks][version]"
	echo ""
}

dateinfo() {
	local flag=0
	echo -e "\t\tDATE"
	echo -e "========================================="
	echo -e localtaime:`date +"%Y-%m-%d %H:%M:%S"`

       # while [[ "x"$ans != "xy" && "x"$ans != "xn" ]]
	#do
		#echo -en "Update the localtime:[y/n]"
		#read ans
	#done
	echo -en "Update the localtime:[y/n]"
	read ans
	if [ "x"$ans == "xy" ];then
		echo -e "ntpdate..."
		#在ntpservers里面添加服务器地址
		ntpservers="11.1.1.1 time-a.nist.gov time-a.timefreq.bldrdoc.gov time.nist.gov time-nw.nist.gov"
		for ntpserver in $ntpservers;
		do
			sudo ntpdate $ntpserver > /dev/null 2>&1
			if [ $? -ne 0 ];then
				continue
			else 
				flag=1
				echo -e "ntpdate success!"
				break
			fi
		done
		if [ $flag -eq 0 ];then
			echo -e "ntpdate failed."
		fi
		echo  -e `date +"%Y-%m-%d %H:%M:%S"`
	fi
	echo -e "=========================================\n"
}

cpuinfo() {
	echo -e "\t\tCPUINFO"
	echo -e "========================================="
	cpuinfo=`sudo cat /proc/cpuinfo|grep "model name"|head -1`
	echo -e "cpu num         : `sudo dmidecode -t processor|grep ID|wc -l`"
	echo -e "$cpuinfo"
	echo -e "=========================================\n"
}

meminfo() {
	echo -e "\t\tMEMINFO"
	echo -e "========================================="
	memnum=`sudo dmidecode -t memory|grep Size:|grep -v "No Module Installed"|wc -l`
	memtype=`sudo dmidecode -t memory|grep Type:|grep -v Error|grep -v Unknow|awk -F : '{print $2}'`
	memsize=`sudo dmidecode -t memory | grep Size:|grep -v "No Module Installed"|awk -F : '{print $2}'`
	memspeed=`sudo dmidecode -t memory|grep Speed:|grep -v Configured|grep -v Unknown|awk -F : '{print $2}'`
	echo -e "total: $memnum"
	echo -e type : $memtype
	echo -e size : $memsize
	echo -e speed: $memspeed
	#echo -e "`free -m`"
	#echo -e "\t\t\t\t\t\t\tunits: M"
	echo -e "=========================================\n"
}

sysdisk(){
	echo -e "\t\tSYSDISK"
	echo -e "========================================="
	sysdisk=`mount|grep "on / "|awk '{print $1}'`
	if [ -L $sysdisk ]; then
		sysdisk=`ls -l $sysdisk|awk '{print $NF}'|tr -d [:digit:]`
	else 
		sysdisk=`basename $sysdisk|tr -d [:digit:]`
	fi
	sysdisksize=`sudo cat /sys/block/$sysdisk/size`
	echo -e "sysdisk: $sysdisk"
	echo -e "size   : $(($sysdisksize*512/1024/1024))M"
	echo -e "==========================================\n"
}

network(){
	echo -e "\t\tNETWORK"
	echo -e "=========================================="
	netcard=`sudo ls -l /sys/class/net|grep "devices"|grep -v "lo"|grep -v "total"|awk '{print $9}'`
	echo -e "total: `echo $netcard | wc -w`"
	#echo -e "$netcard"
	echo -e "==========================================\n"
}

disks(){
	if [ -z $sysdisk ];then
		sysdisk
	fi
	echo -e "\t\tDISKS:"
	echo -e "=========================================="
	disks=`ls -l /sys/block |grep -E ".*/host([0-9]+)/.*/block/sd[a-z]+"|grep -v $sysdisk|awk '{print $9}'`
	disknum=`echo $disks | wc -w`
	readfail=''
	writefail=''
	echo -e "total: $disknum"
	#echo -e "$disks"
	for disk in $disks; do
		dd if=/dev/zero of=/dev/$disk bs=511 count=1 >/dev/null 2>&1
		if [ $? -ne 0 ];then
			writefail=$writefail' '$disk
		fi
		dd if=/dev/$disk of=/dev/null bs=512 count=1 >/dev/null 2>&1
		if [ $? -ne 0 ];then
			readfail=$readfail' '$disk
		fi
	done
	if [[ -z $readfail && -z $writefail ]];then
		echo -e "status: all good"
	fi
	if [ ! -z "$readfail" ]; then
		echo -e "readfail: $readfail"
	fi
	if [ ! -z "$writefail" ]; then
		echo -e "writefail: $writefail"
	fi
	echo -e "==========================================\n"
}


if [ -z $1 ]; then
	dateinfo
	cpuinfo
	meminfo
	sysdisk
	network
	disks
	exit 0
fi

while [ ! -z $1 ];
do 
	case "$1" in
		date)
			dateinfo
			shift
			;;
		cpu)
			cpuinfo
			shift
			;;
		mem)
			meminfo
			shift
			;;
		sysdisk)
			sysdisk
			shift
			;;
		network)
			network
			shift
			;;
		disks)
			disks
			shift
			;;
		version)
			version
			shift
			;;
		*)
			help
			exit 1
			;;
	esac
done
