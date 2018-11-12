
.PHONY: all clean

all: p2 pdf

clean:
	-rm -f p2

p2: Prog2.cu
	nvcc -o prog2 Prog2.cu -std=c++11

pdf: PA2.tex
	latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" -shell-escape -use-make PA2.tex;latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" -shell-escape  -use-make PA2.tex