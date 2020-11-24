CC = gcc
CFLAGS = -Wall
HDR = data.h list.h kdtree.h
SRC1 = map1.c data.c list.c kdtree.c
OBJ1 = $(SRC1: .c=.o)
EXE1 = map1
SRC2 = map2.c data.c list.c kdtree.c
OBJ2 = $(SRC2: .c=.o)
EXE2 = map2

all: $(EXE1)
$(EXE1): $(HDR) $(OBJ1)
	$(CC) $(CFLAGS) -o $(EXE1) $(OBJ1) -lm

all: $(EXE2)
$(EXE2): $(HDR) $(OBJ2)
	$(CC) $(CFLAGS) -o $(EXE2) $(OBJ2) -lm

clean: 
	rm -f $(EXE1) *.o

$(OBJ1): $(HDR)
$(OBJ2): $(HDR)