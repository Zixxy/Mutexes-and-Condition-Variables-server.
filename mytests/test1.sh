#!/bin/sh

cc master.c -o master 2> /dev/null

echo "test sprawdzajacy sygnaly, trwa ok 10 sekund"

./master 5 | sort> out5

if [$(diff out out5) = ""]; then
	echo OK
else
	echo FAIL
fi

