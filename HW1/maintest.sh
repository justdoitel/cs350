#CANT USE WITHOUT /home/cs350001/ppmcvt (his program)

gcc ppmcvt.c pbm_aux.c pbm.h pbm.c -o pain #compile
#bg:i:r:smt:n:o:

/home/cs350001/ppmcvt -o his.pbm Samplei/clock.ppm
pain -o mine.pbm Samplei/clock.ppm
diff his.pbm mine.pbm
/home/cs350001/ppmcvt -o his.pbm Samplei/dooley.ppm
pain -o mine.pbm Samplei/dooley.ppm
diff his.pbm mine.pbm
/home/cs350001/ppmcvt  -o his.pbm Samplei/odd-square.ppm
pain -o mine.pbm Samplei/odd-square.ppm
diff his.pbm mine.pbm
/home/cs350001/ppmcvt -o his.pbm Samplei/square.ppm
pain -o mine.pbm Samplei/square.ppm
diff his.pbm mine.pbm

#test for b - passed
/home/cs350001/ppmcvt -b -o his.pbm Samplei/clock.ppm
pain -b -o mine.pbm Samplei/clock.ppm
diff his.pbm mine.pbm
/home/cs350001/ppmcvt -b -o his.pbm Samplei/dooley.ppm
pain -b -o mine.pbm Samplei/dooley.ppm
diff his.pbm mine.pbm
/home/cs350001/ppmcvt -b -o his.pbm Samplei/odd-square.ppm
pain -b -o mine.pbm Samplei/odd-square.ppm
diff his.pbm mine.pbm
/home/cs350001/ppmcvt -b -o his.pbm Samplei/square.ppm
pain -b -o mine.pbm Samplei/square.ppm
diff his.pbm mine.pbm

#test for g - Passed -- commented out for intesivity
for i in {1..100..1}  #should be 1..65535..1
do
/home/cs350001/ppmcvt -g $i -o his.pgm Samplei/clock.ppm
pain -g $i -o mine.pgm Samplei/clock.ppm
diff his.pgm mine.pgm
/home/cs350001/ppmcvt -g $i -o his.pgm Samplei/dooley.ppm
pain -g $i -o mine.pgm Samplei/dooley.ppm
diff his.pgm mine.pgm
/home/cs350001/ppmcvt -g $i -o his.pgm Samplei/odd-square.ppm
pain -g $i -o mine.pgm Samplei/odd-square.ppm
diff his.pgm mine.pgm
/home/cs350001/ppmcvt -g $i -o his.pgm Samplei/square.ppm
pain -g $i -o mine.pgm Samplei/square.ppm
diff his.pgm mine.pgm
done

#test for i - Passed
for i in "red" "green" "blue" 
do
/home/cs350001/ppmcvt -i $i -o his.ppm Samplei/clock.ppm
pain -i $i -o mine.ppm Samplei/clock.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -i $i -o his.ppm Samplei/dooley.ppm
pain -i $i -o mine.ppm Samplei/dooley.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -i $i -o his.ppm Samplei/odd-square.ppm
pain -i $i -o mine.ppm Samplei/odd-square.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -i $i -o his.ppm Samplei/square.ppm
pain -i $i -o mine.ppm Samplei/square.ppm
diff his.ppm mine.ppm
done

#test for r - Passed
for r in "red" "green" "blue" 
do
/home/cs350001/ppmcvt -r $r -o his.ppm Samplei/clock.ppm
pain -r $r -o mine.ppm Samplei/clock.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -r $r -o his.ppm Samplei/dooley.ppm
pain -r $r -o mine.ppm Samplei/dooley.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -r $r -o his.ppm Samplei/odd-square.ppm
pain -r $r -o mine.ppm Samplei/odd-square.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -r $r -o his.ppm Samplei/square.ppm
pain -r $r -o mine.ppm Samplei/square.ppm
diff his.ppm mine.ppm
done

#test for s - Passed
/home/cs350001/ppmcvt -s -o his.ppm Samplei/clock.ppm
pain -s -o mine.ppm Samplei/clock.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -s -o his.ppm Samplei/dooley.ppm
pain -s -o mine.ppm Samplei/dooley.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -s -o his.ppm Samplei/odd-square.ppm
pain -s -o mine.ppm Samplei/odd-square.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -s -o his.ppm Samplei/square.ppm
pain -s -o mine.ppm Samplei/square.ppm
diff his.ppm mine.ppm

#test for m - Passed
/home/cs350001/ppmcvt -m -o his.ppm Samplei/clock.ppm
pain -m -o mine.ppm Samplei/clock.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -m -o his.ppm Samplei/dooley.ppm
pain -m -o mine.ppm Samplei/dooley.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -m -o his.ppm Samplei/odd-square.ppm
pain -m -o mine.ppm Samplei/odd-square.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -m -o his.ppm Samplei/square.ppm
pain -m -o mine.ppm Samplei/square.ppm
diff his.ppm mine.ppm

#test for t - Passed
for t in {1..8..1} 
do
/home/cs350001/ppmcvt -t $t -o his.ppm Samplei/clock.ppm
pain -t $t -o mine.ppm Samplei/clock.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -t $t -o his.ppm Samplei/dooley.ppm
pain -t $t -o mine.ppm Samplei/dooley.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -t $t -o his.ppm Samplei/odd-square.ppm
pain -t $t -o mine.ppm Samplei/odd-square.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -t $t -o his.ppm Samplei/square.ppm
pain -t $t -o mine.ppm Samplei/square.ppm
diff his.ppm mine.ppm
done



# test for n - Passed
for n in {1..8..1} 
do
/home/cs350001/ppmcvt -n $n -o his.ppm Samplei/clock.ppm
pain -n $n -o mine.ppm Samplei/clock.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -n $n -o his.ppm Samplei/dooley.ppm
pain -n $n -o mine.ppm Samplei/dooley.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -n $n -o his.ppm Samplei/odd-square.ppm
pain -n $n -o mine.ppm Samplei/odd-square.ppm
diff his.ppm mine.ppm
/home/cs350001/ppmcvt -n $n -o his.ppm Samplei/square.ppm
pain -n $n -o mine.ppm Samplei/square.ppm
diff his.ppm mine.ppm
done

echo I AM DONE