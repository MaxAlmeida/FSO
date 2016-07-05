TARGET = LAB.pdf

BIBTEX = bibtex
LATEX = latex
DVIPS = dvips
PS2PDF = ps2pdf

VERSION = 0.1.0

STATIC_DIR = static
STATIC_SOURCES = pageconfig.tex commands.tex packages.tex
STATIC_FILES = $(addprefix $(STATIC_DIR)/, $(STATIC_SOURCES))

BIBLIOGRAPHY_FILE = bibliography.bib

MAIN_FILE = lab-01.tex
DVI_FILE  = $(addsuffix .dvi, $(basename $(MAIN_FILE)))
AUX_FILE  = $(addsuffix .aux, $(basename $(MAIN_FILE)))
PS_FILE   = $(addsuffix .ps, $(basename $(MAIN_FILE)))
PDF_FILE  = $(addsuffix .pdf, $(basename $(MAIN_FILE)))

SOURCES = $(STATIC_FILES)

.PHONY: all clean dist-clean

all: 
	@make $(TARGET)

$(TARGET): $(MAIN_FILE) $(SOURCES)
	$(LATEX) $(MAIN_FILE) $(SOURCES)
	#$(BIBTEX) $(AUX_FILE)
	#$(LATEX) $(MAIN_FILE) $(SOURCES)
	#$(LATEX) $(MAIN_FILE) $(SOURCES)
	$(DVIPS) $(DVI_FILE)
	$(PS2PDF) $(PS_FILE)
	@cp $(PDF_FILE) $(TARGET)

clean:
	rm -f *~ *.dvi *.ps *.backup *.aux *.log
	rm -f *.lof *.lot *.bbl *.blg *.brf *.toc *.idx
	rm -f *.pdf

dist: clean
	tar vczf tcc-fga-latex-$(VERSION).tar.gz *

dist-clean: clean
	rm -f $(PDF_FILE) $(TARGET)
