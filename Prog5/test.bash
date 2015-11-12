for i in {1..15}
do
for j in {1..15}
do
for k in {1..15}
do

echo $i $j $k
./prog5 $i $j $k | python bigtest.py >> output

done
done
done

