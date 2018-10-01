
.PHONY: all clean

all: p1 p2

clean:
	-rm -f p1
	-rm -f p2

p1: Prog1_1.c
	gcc -Wall -fopenmp -o p1 -DGRAD Prog1_1.c

p2: Prog1_2.c
	gcc -Wall -fopenmp -o p2 -DOMP Prog1_2.c -lm

