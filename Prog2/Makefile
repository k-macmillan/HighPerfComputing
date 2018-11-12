
.PHONY: all clean

all: p2

clean:
	-rm -f p2

p2: Prog2.cu
	nvcc -o prog2 Prog2.cu -std=c++11
