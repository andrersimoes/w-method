SRC=main
TEX=latex

.PHONY: dvi pdf lib

all: dvi

dvi: $(SRC).dvi
pdf: $(SRC).pdf

$(SRC).dvi: *.tex lib
	$(TEX) $(SRC).tex
	$(TEX) $(SRC).tex

$(SRC).pdf: $(SRC).dvi
	dvipdfm $(SRC)

lib: *.sty

*.sty: 
	cp lib/*.sty .

clean:
	rm -f *.log *.aux *.dvi

distclean: clean
	rm -f *.sty *.pdf
