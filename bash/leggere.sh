#!/bin/bash
exec {FD}</usr/include/stdio.h
if (( $?==0 ));then
		while read -u ${FD} a b c d;do
				echo $c
		done
fi
