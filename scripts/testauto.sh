#!/bin/bash 
#----- VARIABLE DEFINITIONS --------
PATH_TO_BINARY=/home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/bin
PATH_TO_BUILD=/home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/
PATH_TO_SRC=/home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src 
PATH_TO_SCRIPT=/home/colin/Documents/Courses/multicore-522/Project/NBHashTable/scripts
BINARY_NAME=perfprof
SRC_FILE=perfprof.c
DATAFILE_NAME=perf.csv 
MAX_NUM_THREADS=256
TEST_ITERATIONS=1

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
        alter_nthreads "${PATH_TO_SRC}/${SRC_FILE}" ${nthreads} 
        make 
        if [[ $? -ne 0 ]]; then
            echo "[ERROR]: Build error occurs when doing make"
            exit 1
        fi
        for (( j = 0; j < $1; j++ )); do
            ${PATH_TO_BINARY}/${BINARY_NAME} >> ${PATH_TO_SCRIPT}/${DATAFILE_NAME}
        done
    done

}

## go to BUILD directory 
cd ${PATH_TO_BUILD} 

if (( $# != 1 )); then
    echo " default number of test iterations is 1"
    testloop 1 
else
    echo "you set the number of test iterations to $1"
    testloop $1 
fi



