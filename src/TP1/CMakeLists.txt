cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(AlgoImage_TP1 CXX C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -I")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -pg")


if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir, delete CMakeCache.txt from source root and try again.")
endif()

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_STD_CXX11)
    check_cxx_compiler_flag(-std=c++0x SUPPORTS_STD_CXX01)
    if(SUPPORTS_STD_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
        add_definitions( -D__NO_INLINE__)
    elseif(SUPPORTS_STD_CXX01)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")
    else()
        message(ERROR "Compiler does not support -std=c++11 or -std=c++0x.")
    endif()
endif()

file(
        GLOB_RECURSE
        source_files
        source/*
        include/*
        include/*/*
        ./../../CImg/CImg.h
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(ADDITIONAL_LIBS "mingw32")
endif()

add_executable(AlgoImage_TP1 ${source_files} include/CImg.h)
target_link_libraries(AlgoImage_TP1 m ${ADDITIONAL_LIBS} )

file( GLOB IMG "img/*")
file(COPY ${IMG} DESTINATION ${CMAKE_BINARY_DIR}/img)