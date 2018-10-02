#!/bin/sh

BIN=../Famine

test_with()
{
	echo "Test" $1 $2
	cp $1 /tmp/test/test
	$BIN --recur
	$1 $2 > out

	cp $1 /tmp/test2/test

	echo "  First"
	/tmp/test/test $2 > woody_out
	diff out woody_out || exit 1

	echo "  Second"
	/tmp/test2/test $2 > woody_out
	diff out woody_out || exit 1

	echo "  File"
	(! diff $1 /tmp/test/test) || exit 1
	(! diff $1 /tmp/test2/test) || exit 1

	echo "  Clean"
	rm -rf /tmp/test2/test /tmp/test/test
	rm out woody_out
}

rm -rf /tmp/test
mkdir /tmp/test
rm -rf /tmp/test2
mkdir /tmp/test2

test_with '/bin/tar' '' || exit 1
test_with '/bin/sh' 'data/sh_script' || exit 1
test_with '/usr/bin/diff' 'data/sh_script test.sh' || exit 1
test_with '/bin/ls' '-la ../..' || exit 1

exit 0
