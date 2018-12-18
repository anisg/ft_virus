#!/bin/sh

BIN=../../Death

printStart(){
	echo "Test block: \033[0;32m" $2 "\033[0m" "; $1 tests"
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

test_with()
{
	X=$1
	shift
	printStart 4 "$X $@"
	cp $X /tmp/test/testv
	P=$@
	$BIN --recur # --msg
	$X $P > out
	cp $X /tmp/test2/testv

	printTest "1: test1" "output diff between /tmp/test/testv and $X"
	pkill test
	/tmp/test/testv $P > woody_out || printFail "(test return != 0)" || return 1
	diff out woody_out || printFail "(woody and out differ)" || return 1
	printOk

	printTest "2: test2" "output diff between /tmp/test2/testv and $X"
	/tmp/test2/testv $P > woody_out || printFail || return 1
	diff out woody_out || printFail || return 1
	printOk

	printTest "3: fingerprint" "chek 1 and only 1 finger print"
	echo 1 > /tmp/test/1
	strings /tmp/test/testv | grep ndombre | wc -l | diff - /tmp/test/1
	strings /tmp/test2/testv | grep ndombre | wc -l | diff - /tmp/test/1
	printOk

	printTest "4: diff" "binary diff with $X (original), must be different"
	(! diff $X /tmp/test/testv 1>/dev/null) || printFail "(/tmp/test/testv does not differ from $X)" || return 1
	(! diff $X /tmp/test2/testv 1>/dev/null) || printFail "(/tmp/test2/testv does not differ from $X)" || return 1
	(! diff /tmp/test/testv /tmp/test2/testv 1>/dev/null) || printFail "(/tmp/test/testv does not differ from /tmp/test2/testv)" || return 1
	printOk

	rm -rf /tmp/test2/testv /tmp/test/testv /tmp/test/1
	rm out woody_out
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
	echo "
                                                   _  _
                                                   ' \/ '
   _  _                        <|
    \/              __'__     __'__      __'__
                   /    /    /    /     /    /
                  /\____\    \____\     \____\               _  _
                 / ___!___   ___!___    ___!___               \/
               // (      (  (      (   (      (
             / /   \______\  \______\   \______\ 
           /  /   ____!_____ ___!______ ____!_____
         /   /   /         //         //         /
      /    /   |         ||         ||         |
     /_____/     \        \         \         \ 
           \      \_________\_________\_________\ 
            \         |          |         |
             \________!__________!_________!________/
              \|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_/|
               \    .       .      .       .       /
\033[1;34m^^^^^^^^^^^^^^^^\033[0m\_________________________________/\033[1;34m^^^^^^^^^^^^^^^^^^^^^
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
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

	resetDir

	cp $V /tmp/test/x

	$BIN --recur
	cp /tmp/test/x /tmp/test/x_start

	rec_test $r $V
}

resetDir()
{
	rm -rf /tmp/test
	mkdir /tmp/test
	rm -rf /tmp/test2
	mkdir /tmp/test2
}

#rec_test_init 30 $x
echo ">> TEST #1"

resetDir

test_with '/bin/cat' 'run.sh' || fail
test_with '/bin/sh' 'data/sh_script' || fail
test_with '/usr/bin/diff' 'data/sh_script' 'data/sh_script' || fail
test_with '/bin/ls' '-la' '../..' || fail

#test --recur
echo ">> TEST #2"

resetDir
cp /bin/ls /tmp/test/ls_one
$BIN --recur

printTest "1: infect1" "infect /tmp/test/ls_one and check if its infected"
check_infected /tmp/test/ls_one || printFail || fail
printOk

mkdir /tmp/test/subdir
cp /bin/ls /tmp/test/subdir/ls_two

printTest "2: infect2" "infect ls_two from ls_one ; ls_two is in a subdir"
/tmp/test/./ls_one 2>&1 1>/dev/null
check_infected /tmp/test/subdir/ls_two || printFail || fail
printOk


printTest "3: infect3" "check there is not a reinfection"
/tmp/test/./ls_one 1>/dev/null
check_infected /tmp/test/subdir/ls_two || printFail || fail
printOk

echo ">> TEST #3"

resetDir
cp /bin/ls /tmp/test/ls_one

#$BIN --msg
#printTest "1: test bonus --msg" "check output contains string"
#/tmp/test/./ls_one | head -1 | grep '\[I am a bad' 1>/dev/null 2>/dev/null || printFail || fail
#printOk

resetDir
cp /bin/ls /tmp/test/ls_one

$BIN
printTest "1: test proc" "check not infected when process \'test\' exist"
echo "int main(){while(1){};}" > 'test.c'
gcc 'test.c' -o 'test'
./test &
TEST_PID=$!
cp /bin/ls /tmp/test/ls_two
/tmp/test/ls_one 1>/dev/null 2>/dev/null
kill $TEST_PID
check_infected /tmp/test/ls_two && (printFail "ls_two is infected (from infected binary)" ; fail)
strings /tmp/test/ls_two | grep ndombre
printOk

resetDir
cp /bin/ls /tmp/test/ls_one

echo "1" > /tmp/ok 
strace $BIN 2>&1 | grep 'DEBUGGING...' | wc -l | diff - /tmp/ok || printFail || fail
check_infected /tmp/test/ls_one && (printFail || fail)


#TODO: check from Pestilence?

rm 'test'
rm 'test.c'
exit 0
