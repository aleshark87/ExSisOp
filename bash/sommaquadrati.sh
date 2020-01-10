#!/bin/bash
somma=0
for (( num=1;${num}<$#;num=${num}+1 ));do
		(( quadrato=(${!num}*${!num}) ))
		echo $quadrato	
		(( somma=${somma}+${quadrato} ))
done
echo ${somma}
