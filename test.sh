#!/bin/bash

PROG=""
DIRECTORY=""
FLAG=""

if [[ $3 != "" ]]; then
	FLAG=$1
	PROG=$2
	DIRECTORY=$3
else
	PROG=$1
	DIRECTORY=$2
fi

for f in $DIRECTORY/*.in; do	
	if [[ $FLAG != "" ]]; then 
		./$PROG $FLAG<$f >moj.out 2>moj.err
	else
		./$PROG <$f >moj.out 2>moj.err
	fi
	
	diff ${f%in}out moj.out
	if [[ $? != 0 ]];then
		echo "Zły test " ${f%in}out
	fi

	diff ${f%in}err moj.err
	if [[ $? != 0 ]]; then
		echo "Zły test " ${f%in}err
	fi
done;	
