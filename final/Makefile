
.PHONY: all clean

all: final pdf

clean:
	-rm -f final
	-rm -f *.aux
	-rm -f *.fdb_latexmk
	-rm -f *.fls
	-rm -f *.loa
	-rm -f *.lof
	-rm -f *.log
	-rm -f *.out
	-rm -f *.toc

final: final.cpp
	mpiCC -o final final.cpp -std=c++11

pdf: Final.tex
	latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" -shell-escape -use-make Final.tex
