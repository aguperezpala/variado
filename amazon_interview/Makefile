IDIR =.
CC=g++
CFLAGS=-I$(IDIR) -std=c++11

ODIR=.
LDIR =

LIBS=

_DEPS = Parser.h Debug.h FunctionBuilder.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Parser.cpp FunctionBuilder.cpp main.cpp
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clitool: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(INCDIR)/*~ 