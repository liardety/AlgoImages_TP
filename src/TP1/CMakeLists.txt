cmake_minimum_required(VERSION 3.0.2)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(AlgoImage CXX C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -O3 -std=c++11")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -pg -g")


if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir, delete CMakeCache.txt from source root and try again.")
endif()
file(
        GLOB_RECURSE
        source_files
        source/*
        include/*
)
include_directories(
        include
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(ADDITIONAL_LIBS "mingw32")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math -I /usr/X11R6/include -I ./CImg")
    set(CMAKE_EXE_LINKER_FLAGS "-L . -L /usr/X11R6/lib  -lpthread -lX11 -lXext -Dcimg_use_xshm  -lm")
endif()

add_executable(AlgoImage ${source_files} include/Kernel.h include/Distance.h include/KFCM.h sample/KFCM.cpp include/EuclidianDistance.h)


file(GLOB IMG "img/*")
file(COPY ${IMG} DESTINATION ${CMAKE_BINARY_DIR}/img)

