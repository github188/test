#!/usr/bin/env python
# -*- coding: utf-8 -*-

from libmontest import mon_add

def do_network_test():
	mon_add('网口eth0', 'eth', 'eth0,target_ip=192.168.70.1')
	mon_add('网口eth1', 'eth', 'eth1,target_ip=192.168.70.1')
	mon_add('网口eth2', 'eth', 'eth2,target_ip=192.168.70.1')
	mon_add('网口eth3', 'eth', 'eth3,target_ip=192.168.70.1')
