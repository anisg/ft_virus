#!/bin/sh

BIN=../woody_woodpacker

test_with()
{
	echo $BIN $1
	$BIN $1
	echo "....WOODY...." > out
	$1 $2 >> out
	./woody $2 > woody_out
	diff out woody_out
	rm woody out woody_out
}

test_with '/bin/tar' '' || exit 1
test_with '/bin/sh' 'data/sh_script' || exit 1
test_with '/usr/bin/diff' 'data/sh_script test.sh' || exit 1
test_with '/bin/ls' '-la ..' || exit 1
exit 0
