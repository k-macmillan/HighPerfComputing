
.PHONY: all clean

all: final pdf

clean:
	-rm -f final
	-rm -f *.loa
	-rm -f *.lof
	-rm -f *.log
	-rm -f *.toc

final: final.cpp
	mpiCC -o final final.cpp -std=c++11

pdf: Final.tex
	latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" -shell-escape -use-make Final.tex;latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" -shell-escape -use-make Final.tex