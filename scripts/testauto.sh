#!/bin/bash 
#----- VARIABLE DEFINITIONS --------
## set project root manually here
PROJ_ROOT="${HOME}/Documents/Courses/multicore-522/Project/NBHashTable"
PATH_TO_BINARY=${PROJ_ROOT}/build/bin
PATH_TO_BUILD=${PROJ_ROOT}/build/
PATH_TO_SRC=${PROJ_ROOT}/src 
PATH_TO_SCRIPT=${PROJ_ROOT}/scripts
BINARY_NAME=perfprof
SRC_FILE=perfprof.c
DATAFILE_NAME=perf.csv 
MAX_NUM_THREADS=256
TEST_ITERATIONS=1
### This function is never used any more
alter_nthreads ()
{
    if [[ $# -ne 2 ]]; then
        echo "Please provide two arguments, first is the filename and second is the target number of threads"
        exit 1
    fi
    if [[ -z $(egrep "define NUM_THREADS" $1) ]]; then
        echo "not found NUM_THREADS macro "
        exit 1
    fi
    #alter the number of threads in source file 
    sed -i "s@\(#define NUM_THREADS \+\)[0-9]\+@\1$2@" "$1"

}

testloop()
{
    
    ## delete old data output file if exists 
    if [[ -e ${PATH_TO_SCRIPT}/${DATAFILE_NAME} ]]; then
        rm ${PATH_TO_SCRIPT}/${DATAFILE_NAME}
    fi

    ## create new data file to store output 
    echo "Implementation, Workload, Number_of_threads, Time[ms]" > ${PATH_TO_SCRIPT}/${DATAFILE_NAME}

    for (( nthreads = 2; nthreads <= ${MAX_NUM_THREADS}; nthreads *= 2 )); do
        for (( testid = 0; testid < 3; testid++ )); do
            ${PATH_TO_BINARY}/${BINARY_NAME} $nthreads $testid >> ${PATH_TO_SCRIPT}/${DATAFILE_NAME}
        done
    done

}

## go to BUILD directory 
cd ${PATH_TO_BUILD} 
make 
if [[ $? -ne 0 ]]; then
    echo "[ERROR]: Build error occurs when doing make"
    exit 1
fi


testloop
