#!/bin/bash
echo Automatic tagdb updater

while true; do
	echo Building tags...

	startTime=$(date +%s)

	cscope -R -b
	ctags -R --c-kinds=+p --fields=+iaS --extra=+q -R .

	endTime=$(date +%s)
	diff=$(($endTime - $startTime))

	echo "Tags generated in $diff seconds."

	cloc --quiet --no3 --yaml src/ | tail -4

	echo "----"
	sleep 1m
done
