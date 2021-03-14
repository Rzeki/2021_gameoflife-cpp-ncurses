.SUFFIXES: .cxx .hxx
DIR = `basename $(CURDIR)

GAMEOFLIFE = GameofLife

CGAMEOFLIFE = $(GAMEOFLIFE).cxx

XGAMEOFLIFE = $(GAMEOFLIFE).x

HEADER1 = GameofLife.hxx

CFLAGS = -lncurses

CXX = g++

.PHONY: all tar clean run

all: $(XGAMEOFLIFE)

$(XGAMEOFLIFE): $(CGAMEOFLIFE) $(HEADER1)
	$(CXX) $< $(CFLAGS) -o $(XGAMEOFLIFE) 

run: all
	./$(XGAMEOFLIFE)

clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out

tar: clean
	(cd ../;tar zcvf $(DIR).tar.gz $(DIR))
