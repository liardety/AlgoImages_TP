cmake_minimum_required(VERSION 3.3)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(AlgoImage CXX C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -O2")
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

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(ADDITIONAL_LIBS "mingw32")
endif()

add_executable(AlgoImage ${source_files})

file(GLOB IMG "img/*")
file(COPY ${IMG} DESTINATION ${CMAKE_BINARY_DIR}/img)