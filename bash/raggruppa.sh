#!/bin/bash
FILEAPPOGGIO=tmp.txt
if [[ -e ${FILEAPPOGGIO} ]];
then
		rm -f ${FILEAPPOGGIO}
fi
while read DATA LUOGO MOTIVO DANNI; do
		NUM=`grep ${MOTIVO} cadutevic.txt | wc -l`
		echo "${MOTIVO} ${NUM}" >> ${FILEAPPOGGIO}
done < cadutevic.txt

sort ${FILEAPPOGGIO} | uniq

exit 0

