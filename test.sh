rm -rf /tmp/test/*

cp /bin/ls /tmp/test/ls1
./Death
cp /bin/ls /tmp/test/ls2
echo "STRING1"
strings /tmp/test/ls1 | grep 'ndombre'
/tmp/test/./ls1
echo "STRING2"
strings /tmp/test/ls2 | grep 'ndombre'
