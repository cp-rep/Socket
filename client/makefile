BINDIR=./bin
SRCDIR=./src
MKDIR_P=mkdir -p
LDIR=./lib
ODIR=obj
CC=gcc
CPPFLAGS=-I$(IDIR)
LIBS=-lm
BINNAME=client
_DEPS=  sockets.h
_OBJ = main.o sockets.o

DEPS = $(patsubst %,$(LDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(SRCDIR)/$(ODIR)/%,$(_OBJ))

$(SRCDIR)/$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
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
