CC := gcc
CCFLAGS := -g -Wall
WITHOMPFLAG := $(CCFLAGS) -fopenmp 
OBJ := testHashMap.o nbhash.o
HEADERS := nbhash.h def.h
# Comment out for log-free execution
CCFLAGS += -DDEBUG
test:${OBJ}
	$(CC) $(WITHOMPFLAG) -o $@ $(OBJ)

nbhash.o:nbhash.c $(HEADERS)
	$(CC) -c $(CCFLAGS) nbhash.c
testHashMap.o:testHashMap.c $(HEADERS)
	$(CC) -c $(WITHOMPFLAG) testHashMap.c 



.PHONY:clean

clean:
	-rm test $(OBJ) 
