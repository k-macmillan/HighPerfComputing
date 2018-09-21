# You want latexmk to *always* run, because make does not have all the info.
# Also, include non-file targets in .PHONY so they are run regardless of any
# file of the given name existing.
.PHONY: HW1.pdf all clean

# The first rule in a Makefile is the one executed by default ("make"). It
# should always be the "all" rule, so that "make" and "make all" are identical.
all: HW1.pdf

# CUSTOM BUILD RULES

# In case you didn't know, '$@' is a variable holding the name of the target,
# and '$<' is a variable holding the (first) dependency of a rule.
# "raw2tex" and "dat2tex" are just placeholders for whatever custom steps
# you might have.

%.tex: %.raw
		./raw2tex $< > $@

%.tex: %.dat
		./dat2tex $< > $@

# MAIN LATEXMK RULE

# -pdf tells latexmk to generate PDF directly (instead of DVI).
# -pdflatex="" tells latexmk to call a specific backend with specific options.
# -use-make tells latexmk to call make for generating missing files.

# -interaction=nonstopmode keeps the pdflatex backend from stopping at a
# missing file reference and interactively asking you for an alternative.

HW1.pdf: HW1.tex
		# Run it twice so it gathers references otherwise Table of Contents may be blank
		latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" -use-make HW1.tex;latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" -use-make HW1.tex

#need to run pdflatex twice to get refrences.
#pdflatex -interaction-nonstopmode -use-make HW1.tex		
#pdflatex -interaction-nonstopmode -use-make HW1.tex		


clean:
		latexmk -CA
