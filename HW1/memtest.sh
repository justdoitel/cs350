#!/bin/bash

gcc ppmcvt.c pbm_aux.c pbm.h pbm.c -o pain
valgrind --leak-check=full --log-file="a.out" -v pain -o mine.pbm Samplei/dooley.ppm
valgrind --leak-check=full --log-file="b.out" -v pain -b -o mine.pbm Samplei/dooley.ppm
valgrind --leak-check=full --log-file="c.out" -v pain -g 36 -o mine.pgm Samplei/dooley.ppm
valgrind --leak-check=full --log-file="d.out" -v pain -i green -o mine.ppm Samplei/dooley.ppm
valgrind --leak-check=full --log-file="e.out" -v pain -r blue -o mine.ppm Samplei/dooley.ppm
valgrind --leak-check=full --log-file="f.out" -v pain -s -o mine.ppm Samplei/dooley.ppm
valgrind --leak-check=full --log-file="g.out" -v pain -m -o mine.ppm Samplei/dooley.ppm
valgrind --leak-check=full --log-file="h.out" -v pain -t 5 -o mine.ppm Samplei/dooley.ppm
valgrind --leak-check=full --log-file="i.out" -v pain -n 4 -o mine.ppm Samplei/dooley.ppm

