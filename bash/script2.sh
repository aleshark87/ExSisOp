#! /bin/bash
NUM=$#
while(( ${NUM} > "0" ));do
		echo "arg ${NUM} is ${!NUM}"
		((NUM=${NUM}-1))
done
