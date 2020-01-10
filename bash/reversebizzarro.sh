#!/bin/bash
if (( $# != 1 ));
then 
	echo "numero arg sbagliati"
	exit 1
fi
REVERSE=""
numChar=0
echo `ps`
echo $1 | while (( 1 ));do
	read -n 1 a;
	if (( $? == 0 ));
	then
		echo `ps`
		REVERSE="${a}${REVERSE}"
	else
		echo stringa rovesciata= ${REVERSE}
		break
	fi
done


