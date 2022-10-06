exe=c-
path="testData"

for testfile in $path/*.c-;
do
    echo $testfile
    ./$exe -p $testfile > myout
    diff -y myout ${testfile::-3}.out
done
