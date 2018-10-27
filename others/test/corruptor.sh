#!/bin/bash

# Same as "dissect.sh" but without stdin

### Test woody_woodpacker on truncated files
### Run this in a temporary directory, like /tmp

#set -e # Stop on error

if [ -z "$1" ] || [ -z "$2" ]
then
	echo "Usage: $0 ./BIN file"
   	exit 1
fi

bin=$1
filename=$2

# Retrieve size and trim spaces
size=$(wc -c < "$filename" | xargs)

echo $filename $size
cp $filename > /tmp/ok
for (( i=$size; i>0; i--))
do
	rm -rf /tmp/test
	mkdir /tmp/test
	outname="/tmp/test/part$i.__tmp"
	head -c $i "$filename" > "$outname"
	$bin
	retval=$?
	echo -n "Part $i: "
	rm $outname
	if [ $retval -ne 0 ]
	then
		if [ $retval -ne 255 ]
		then
			echo "Fatal error: exit status $retval"
			exit
		else
			echo "Exit 255"
		fi
	else
		echo "Ok"
	fi
done
