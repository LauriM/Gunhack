#!/bin/bash
echo "Automatic tagdb updater"
echo "#######################"

#TODO: Consider before/after stats
#TODO: Git commit counts

while true; do
	echo Building tags,static analysis and premake...

	startTime=$(date +%s)

	premake4 gmake
	cscope -R -b
	ctags -R --c-kinds=+p --fields=+iaS --extra=+q -R .

	make clean && scan-build -o /var/www/scan-build/ make

	endTime=$(date +%s)
	diff=$(($endTime - $startTime))

	echo "-->Tags generated in $diff seconds."

	cloc --quiet --no3 --yaml src/ | tail -4

	echo "----"
	sleep 1m
done
