#!/bin/bash
NUM=0
str=""
for ((NUM=2;${NUM}<=$#;NUM=${NUM}+2));do
	str="${str}${!NUM}";
done
for ((NUM=1;${NUM}<=$#;NUM=${NUM}+2));do
	str="${str}${!NUM}";
done
echo ${str}
