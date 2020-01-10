#!/bin/bash
if (( "$#" == "0" ))
then
	echo "pochi argomenti"
	exit 1
fi
if (( "$#" > "9" ))
then
	echo "troppi argomenti"
	exit 1
fi
num=1
counterpari=0
counterdispari=0
while (( "${num}" <= "$#" ))
do
	righe=`wc -l ${!num}`
	if (( ${num} == 0 ));
	then
		(( counterpari=${counterpari}+${righe} ))
	else
		(( counterdispari=${counterdispari}+${righe} ))
	fi

	(( num=${num}+1 ))
done

echo ${counterpari}
echo ${counterdispari} 1>&2
exit 0

