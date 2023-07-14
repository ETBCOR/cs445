exe=c-
path="data"
diffile="diff.txt"
linesDiff=0
linesTotal=0

echo "Running mock testing harness with the path $path"
echo ""

rm -f $diffile

for infile in $path/*.c-;
do
    outfile=${infile::-3}.out

    echo $infile >> $diffile

    rm -f myout
    
    echo "====================================" >> myout
    echo "FILE: `basename $infile`" >> myout

    ./$exe -P $infile >> myout
    diff -y --suppress-common-lines -W 200 myout $outfile >> $diffile
    
    linesDiff=$(($linesDiff + `sdiff -B -b -s myout $outfile | wc -l`))
    linesTotal=$(($linesTotal + `cat $outfile | wc -l`))


    #cat ${testfile::-3}.out | wc
    #diff -y -W 200 myout ${testfile::-3}.out
    echo "============================================================================================================" >> $diffile
done
rm myout

echo $linesDiff / $linesTotal lines differed in total. >> $diffile
cat $diffile

