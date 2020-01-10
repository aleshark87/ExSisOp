#!/bin/bash

OUT=""
while read a b ALTRO;do
	OUT=${OUT}${b}
done < testo.txt 
echo $OUT
