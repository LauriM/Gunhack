#!/bin/bash

if [$1 = 1]
then
	while true; do
		echo Building tags,static analysis and premake...

		startTime=$(date +%s)

		premake4 gmake
		cscope -R -b
		ctags -R --c-kinds=+p --fields=+iaS --extra=+q -R .


		endTime=$(date +%s)
		diff=$(($endTime - $startTime))

		echo "-->Tags generated in $diff seconds."

		cloc --quiet --no3 --yaml src/ | tail -4

		echo "----"
		sleep 1m
	done
fi

echo "Commands"
echo "stats - displays project stats"
echo "update - Runs ctags/cscope tag updates"
echo "analyse - analyses the source code"
