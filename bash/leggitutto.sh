#!/bin/bash
exec {FD}</home/aless/Desktop/bash/testo.txt
if (( $?==0 ));
then
	while read -u ${FD} RIGA;do
			echo $RIGA
	done
	exec {FD}>&-
fi

