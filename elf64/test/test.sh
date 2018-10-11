#!/bin/sh

BIN=../Famine

exe () {
	FILE=$1
	PARAMS=$2
	OUT_FILE=$3
	echo "run: $FILE $PARAMS > $OUT_FILE"
	$FILE $PARAMS > $OUT_FILE
	return 0
}

test_with()
{
	echo "Test" $1 $2
	cp $1 /tmp/test/test
	echo "infect on test"
	$BIN --recur
	$1 $2 > out
	P=$2
	cp $1 /tmp/test2/test

	exe /tmp/test/test "$P" woody_out || return 1
	diff out woody_out || return 1

	exe /tmp/test2/test "$P" woody_out || return 1
	diff out woody_out || return 1

	echo 1 > /tmp/test/1
	strings /tmp/test/test | grep ndombre | wc -l | diff - /tmp/test/1
	strings /tmp/test2/test | grep ndombre | wc -l | diff - /tmp/test/1

	(! diff $1 /tmp/test/test) || return 1
	(! diff $1 /tmp/test2/test) || return 1

	rm -rf /tmp/test2/test /tmp/test/test
	rm out woody_out
}

check_infected()
{
	FILE=$1
	echo "Test2 $1"
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

simple_test()
{
	INF=$1
	strings $INF | grep 'ndombre' || fail
	$INF
	X=$?
	if (( $X >= 100 )); then
		echo "SEGV OR OTHER"
		return 1
	fi
}

rec_test()
{
	r=$1
	V=$2
	if [ "$r" -eq "0" ]; then return 0; fi

	echo "test! " $2
	rm -rf /tmp/test2/*

	cp $V /tmp/test2/y

	/tmp/test/./x

	simple_test /tmp/test2/y

	rm /tmp/test/x
	mv /tmp/test2/y /tmp/test/x

	let "r--"
	rec_test $r $V
}

rec_test_init()
{
	r=$1
	V=$2

	rm -rf /tmp/test
	rm -rf /tmp/test2
	mkdir /tmp/test
	mkdir /tmp/test2

	cp $V /tmp/test/x

	$BIN --recur
	cp /tmp/test/x /tmp/test/x_start

	rec_test $r $V
}

#rec_test_init 30 $x
echo ">> TEST #1"

rm -rf /tmp/test
mkdir /tmp/test
rm -rf /tmp/test2
mkdir /tmp/test2

test_with '/bin/tar' '' || fail
test_with '/bin/sh' 'data/sh_script' || fail
test_with '/usr/bin/diff' 'data/sh_script test.sh' || fail
test_with '/bin/ls' '-la ../..' || fail

#test --recur
echo ">> TEST #2"
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
