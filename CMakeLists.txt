cmake_minimum_required(VERSION 3.3)
PROJECT(NBHashTable C)
SET(CMAKE_C_FLAGS "-g -Wall -std=gnu99 -fopenmp" )
ADD_SUBDIRECTORY(src)
