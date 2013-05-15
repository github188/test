#!/usr/bin/env bash

for x in `ps ax | grep test-raid.sh | awk '{print $1}'`
do
	kill $x
done

for x in `ps ax | grep kill-idle.sh | awk '{print $1}'`
do
	kill $x
done

for x in `ps ax | grep xdd | awk '{print $1}'`
do
	kill $x
done

for x in `ps ax | grep yes | awk '{print $1}'`
do
	kill $x
done

for x in `ipcs -s | awk '{print $2}'`
do
	ipcrm -s $x
done
