#!/bin/bash
if (( $# > 5 ));
then
	echo troppi argomenti
	exit 1
fi
newest=$1
num=1
while (( num <= $# ));do
#		echo ${!num}
		if [[ ${!num} -nt ${newest} ]];
		then
#			echo cambio newest con ${!num}
			newest=${!num}
		fi
		(( num=${num}+1 ))
done
echo $newest

