#!/bin/bash
while read UNO DUE TRE QUATTRO;do
	if (( $?==0 ))
	then
		echo "${QUATTRO}" "${TRE}"
	fi
done
