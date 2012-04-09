#!/bin/bash
echo "Automatic tagdb updater"
echo "#######################"

while true; do
	echo Building tags and premake...

	startTime=$(date +%s)

	~/premake/premake4 gmake
	cscope -R -b
	ctags -R --c-kinds=+p --fields=+iaS --extra=+q -R .

	endTime=$(date +%s)
	diff=$(($endTime - $startTime))

	echo "Tags generated in $diff seconds."

	cloc --quiet --no3 --yaml src/ | tail -4

	echo "----"
	sleep 1m
done
