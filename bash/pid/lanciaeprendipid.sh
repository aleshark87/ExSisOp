#!/bin/bash
num=0
while (( $num <= 10 ));do
	.././puntini.sh 10 1>&2 &
	echo -n "$! "
done
