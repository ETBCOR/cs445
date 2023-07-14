exe=c-
path="data"

for testfile in $path/*.c-;
do
    echo \n$testfile
    ./$exe -P $testfile > myout
    diff -y --suppress-common-lines -W 200 myout ${testfile::-3}.out
    #diff -y -W 200 myout ${testfile::-3}.out
done
