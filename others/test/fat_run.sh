#!/bin/sh

BIN=../../Pestilence

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

echo ">> #1 LAUNCH"
