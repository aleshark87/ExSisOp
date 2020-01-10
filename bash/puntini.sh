#!/bin/bash
num=0
while (( $num <= $1 ));do
	sleep 1
	echo -n ". $$BASHPID"
	(( num=$num+1 ))
done
