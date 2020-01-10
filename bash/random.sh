#!/bin/bash
num=0
while (( $RANDOM % 10 != 2 ))
do
		(( num=$num + 1 ))
done
echo $num
