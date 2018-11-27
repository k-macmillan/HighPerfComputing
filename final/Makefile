
.PHONY: all clean

all: final pdf

clean:
	-rm -f final
	-rm -f *.toc
	-rm -f *.lof

final: Prog2.cu
	nvcc -o final final.cpp -std=c++11

pdf: final.tex
	latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" -shell-escape -use-make final.tex;latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" -shell-escape  -use-make final.tex