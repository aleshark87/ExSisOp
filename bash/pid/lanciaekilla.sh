#!/bin/bash
PIDS=`./lanciaeprendipid.sh`
echo $PIDS
for pid in $PIDS;do
	kill -9 $pid
done
