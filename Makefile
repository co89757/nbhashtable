CC := gcc
CCFLAGS := -g -Wall
WITHOMPFLAG := $(CCFLAGS) -fopenmp 
OBJ := testHashMap.o nbhash.o
HEADERS := nbhash.h def.h
test:${OBJ}
	$(CC) $(CCFLAGS) -o $@ $(OBJ)

nbhash.o:nbhash.c $(HEADERS)
	$(CC) -c $(CCFLAGS) nbhash.c
testHashMap.o:testHashMap.c $(HEADERS)
	$(CC) -c $(CCFLAGS) testHashMap.c 



.PHONY:clean

clean:
	-rm test $(OBJ) 
