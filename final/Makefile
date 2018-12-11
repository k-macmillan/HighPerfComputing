INCL := -Iinclude
SRC_FILES := $(wildcard src/*.cpp)
CFLAGS := -std=c++11 -Wall
DFLAG := -DSEQUENTIAL
EXECUTABLE = final
EXEC := -o $(EXECUTABLE)
PAPER := paper/
TEX := Final.tex

.PHONY: all clean

all: final pdf

clean:
	-rm -f final

final: $(SRC_FILES)
	mpic++ $(EXEC) $(SRC_FILES) $(INCL) $(CFLAGS)

seqfinal: $(SRC_FILES)
	g++ $(EXEC) $(SRC_FILES) $(INCL) $(CFLAGS) $(DFLAG)

pdf: $(PAPER)$(TEX)
	(cd paper && latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" --shell-escape $(TEX) && latexmk -c)