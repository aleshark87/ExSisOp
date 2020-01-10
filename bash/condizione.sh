#! /bin/bash 
num=33
while(( (num=(${num})*${num}%10)!=2)); do
		echo ${num};
done
