#!/bin/bash
exec {FD}</usr/include/stdio.h
sum=0
if (( $?==0 ));then

	while read -u ${FD} -N 1  a;do
			(( sum=${sum}+1 ))
	done
	exec {FD}>&-

fi
echo $sum	
