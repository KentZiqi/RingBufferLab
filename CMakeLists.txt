cmake_minimum_required(VERSION 3.1)
project(ringbuffer_lab)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Makefile
    ringbuf
    ringbuf.c
    ringbuf.pdf
    testinput0.txt
    testinput1.txt
    testinput2.txt
    testinput3.txt
    testinput4.txt)

add_executable(ringbuffer_lab ${SOURCE_FILES})