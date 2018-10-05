#!/bin/sh

BIN=../Famine

test_with()
{
	echo "Test" $1 $2
	cp $1 /tmp/test/test
	$BIN --recur # --msg
	$1 $2 > out

	cp $1 /tmp/test2/test

	/tmp/test/test $2 > woody_out
	diff out woody_out || return 1

	/tmp/test2/test $2 > woody_out
	diff out woody_out || return 1

	(! diff $1 /tmp/test/test) || return 1
	(! diff $1 /tmp/test2/test) || return 1

	rm -rf /tmp/test2/test /tmp/test/test
	rm out woody_out
}

check_infected()
{
	FILE=$1
	strings $FILE | grep 'ndombre' || return 1
}

fail()
{
	echo "\033[0;31mFATAL ERROR\033[0m"
	echo "\033[0;36m 
        |\ 
        | \  
        |  \ 
        |___\ 
_____\--|----/______ 
      \_____/ 
\033[0m\n(here, a peaceful boat)"
	exit 1
}

rm -rf /tmp/test
mkdir /tmp/test
rm -rf /tmp/test2
mkdir /tmp/test2

test_with '/bin/tar' '' || fail
test_with '/bin/sh' 'data/sh_script' || fail
test_with '/usr/bin/diff' 'data/sh_script test.sh' || fail
test_with '/bin/ls' '-la ../..' || fail

#test --recur
rm -rf /tmp/test
mkdir /tmp/test
cp /bin/ls /tmp/test/ls_one
$BIN --recur

check_infected /tmp/test/ls_one || fail

mkdir /tmp/test/subdir
cp /bin/ls /tmp/test/subdir/ls_two

/tmp/test/./ls_one
check_infected /tmp/test/subdir/ls_two || fail

exit 0
