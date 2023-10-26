BINDIR=./bin
SRCDIR=./src
BUILDDIR=./build
MKDIR_P=mkdir -p
LDIR=./lib
ODIR=obj
CC=g++
CPPFLAGS=-I$(IDIR)
LIBS=-lm
BINNAME=makefile
_DEPS = 
_OBJ = main.o

DEPS = $(patsubst %,$(LDIR)/%,$(_DEPS))

OBJ = $(patsubst %,$(SRCDIR)/$(ODIR)/%,$(_OBJ))

$(SRCDIR)/$(ODIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< -I$(LDIR)

$(BINDIR)/$(BINNAME): $(OBJ)
	$(CC) -o $@ $^ -I$(LDIR) $(LIBS)

.PHONY: clean dirs

clean:
	rm -f $(SRCDIR)/$(ODIR)/*.o
	rm -f $(SRCDIR)/$(ODIR)/*~
	rm -f $(SRCDIR)/*~
	rm -f $(LDIR)/*~
	rm -rf $(BINDIR)/client
	rm -f ./*~

dirs: ${BINDIR} ${SRCDIR}/${ODIR}

${BINDIR}:
	${MKDIR_P} ${BINDIR}

${SRCDIR}/${ODIR}:
	${MKDIR_P} ${SRCDIR}/${ODIR}
