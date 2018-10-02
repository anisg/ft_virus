#!/bin/sh

BIN=../Famine

test_with()
{
	cp $1 /tmp/test
	rm -rf /tmp/test2
	$BIN # --recure
	$1 $2 >> out

	cp $1 /tmp/test2

	/tmp/test $2 > woody_out
	diff out woody_out || exit 1

	/tmp/test2 $2 > woody_out
	diff out woody_out || exit 1

	#cp /usr/bin/vim /tmp/test2
	#cp /usr/bin/vim /tmp/test
	(! diff $1 /tmp/test) || exit 1
	(! diff $1 /tmp/test2) || exit 1

	rm -rf /tmp/test2 /tmp/test
	rm out woody_out
}

test_with '/bin/tar' '' || exit 1
test_with '/bin/sh' 'data/sh_script' || exit 1
test_with '/usr/bin/diff' 'data/sh_script test.sh' || exit 1
test_with '/bin/ls' '-la ../..' || exit 1

exit 0
