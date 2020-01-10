#!/bin/bash

read a b c d;
if [[ $a -nt $b ]];
then
	newest=$a
else
	newest=$b
fi
if [[ $c -nt $newest ]];
then
	newest=$c
else
	if [[ $c -nt $d ]];
	then
		echo $c
	else
		echo $d
	fi
fi
echo $b

