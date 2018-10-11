#!/bin/sh

BIN=../Pestilence

printStart(){
	echo "Start Test: \033[0;32m" $2 "\033[0m" "; $1 tests"
}

printTest(){
	#1 is name, 2 is explanation
	echo "\033[0;33m$1\033[0m ( $2 ) \c"
}

printOk(){
	echo "\033[0;32mOK\033[0m"
}

printFail(){
	echo "\033[0;31mFAIL $1\033[0m"
	return 1
}

printEnd(){
	echo "\033[0;32mEnd Test: " $@ "\033[0m"
}

test_with()
{
	X=$1
	shift
	printStart 3 "$X $@"
	cp $X /tmp/test/test
	P=$@
	$BIN --recur # --msg
	$X $P > out
	cp $X /tmp/test2/test

	printTest "1: test1" "output diff between /tmp/test/test and $X"
	/tmp/test/test $P > woody_out || printFail "(test return != 0)" || return 1
	diff out woody_out || printFail "(woody and out differ)" || return 1
	printOk

	printTest "2: test2" "output diff between /tmp/test2/test and $X"
	/tmp/test2/test $P > woody_out || printFail || return 1
	diff out woody_out || printFail || return 1
	printOk

	printTest "3: diff" "binary diff between /test and /test2 different from $X"
	(! diff $X /tmp/test/test 1>/dev/null) || printFail "(/tmp/test/test differ from $X)" || return 1
	(! diff $X /tmp/test2/test 1>/dev/null) || printFail "(/tmp/test2/test differ from $X)" || return 1
	printOk

	rm -rf /tmp/test2/test /tmp/test/test
	rm out woody_out
	printEnd
}

check_infected()
{
	FILE=$1
	X=`strings $FILE | grep 'ndombre' | wc -l`
	if [ $X -eq 1 ]; then return 0; fi
	return 1
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

test_with '/bin/cat' 'test.sh' || fail
test_with '/bin/sh' 'data/sh_script' || fail
test_with '/usr/bin/diff' 'data/sh_script' 'data/sh_script' || fail
test_with '/bin/ls' '-la' '../..' || fail

#test --recur
echo ">> TEST #2"

rm -rf /tmp/test
mkdir /tmp/test
cp /bin/ls /tmp/test/ls_one
$BIN --recur

printTest "1: infect1" "infect /tmp/test/ls_one and check if its infected"
check_infected /tmp/test/ls_one || printFail || fail
printOk

mkdir /tmp/test/subdir
cp /bin/ls /tmp/test/subdir/ls_two

printTest "1: infect2" "infect ls_two from ls_one"
/tmp/test/./ls_one 2>&1 1>/dev/null
check_infected /tmp/test/subdir/ls_two || printFail || fail
printOk


printTest "1: infect3" "check there is not a reinfection"
/tmp/test/./ls_one 1>/dev/null
check_infected /tmp/test/subdir/ls_two || printFail || fail
printOk

exit 0
