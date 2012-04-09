#!/bin/bash

while true; do
	echo Building tags...

	startTime=$(date +%s)

	cscope -R -b
	ctags -R --c-kinds=+p --fields=+iaS --extra=+q -R .

	endTime=$(date +%s)
	diff=$(($endTime - $startTime))

	echo "done in $diff seconds. Sleeping."
	sleep 1m
done
