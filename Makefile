LD=g++
LDFLAGS=
CXX=g++
CXXFLAGS=-std=c++20 -Wall -pedantic -Wno-long-long -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/libpng16 -I/usr/include/libmount -I/usr/include/blkid
LIBS=-lnotify -pthread -lmpdclient


SRC=$(shell find src -name '*.cpp')
HEAD=$(shell find src -name '*.h')
OBJ=$(patsubst src%, build%, $(patsubst %.cpp, %.o, $(SRC)))

OBJDIRS=$(dir $(OBJ))
DUMMY:=$(shell mkdir --parents $(OBJDIRS))

BIN=mp-notify

## Compile
.PHONY: all
all: $(BIN)

$(BIN): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

run: $(BIN)
	./$(BIN)

## Meta
.PHONY: doc clean

doc: $(SRC) $(HEAD) Doxyfile
	doxygen

clean:
	rm -frd $(OBJ) $(BIN) Makefile.d doc

## Rules
build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

Makefile.d: $(SRC) $(HEAD)
	$(CXX) -MM $(SRC) | sed -E 's/(^.*\.o:)/build\/\1/' > $@

-include Makefile.d
