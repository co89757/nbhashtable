CC := gcc
CCFLAGS := -g -Wall
WITHOMPFLAG := $(CCFLAGS) -fopenmp 
OBJ := testHashMap.o nbhash.o collection.o 

HEADERS := nbhash.h def.h collection.h 

LOGFILE := $(wildcard *.log)
OBJS := $(wildcard *.o)  
# Comment out for log-free execution
CCFLAGS += -DDEBUG
target:${OBJ}
	$(CC) $(WITHOMPFLAG) -o $@ $(OBJ)

collection.o:collection.c def.h 
	$(CC) -c $(CCFLAGS) collection.c 

nbhash.o:nbhash.c def.h 
	$(CC) -c $(CCFLAGS) nbhash.c

testHashMap.o:testHashMap.c $(HEADERS)
	$(CC) -c $(WITHOMPFLAG) testHashMap.c 



.PHONY:clean cleanlog
cleanlog:
	-rm $(LOGFILE)
clean:
	-rm target $(OBJS) $(LOGFILE)
# runtest:
# 	cut -d, -f2 < debug.log | sort -n > sorted.log 
