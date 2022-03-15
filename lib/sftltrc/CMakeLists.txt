cmake_minimum_required(VERSION 3.16)
project(sftltrc CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-security")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-int-to-void-pointer-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

include_directories(./include/)

file(GLOB src_main ./include/sftltrc/sftltrc.c)

add_library(sftltrc ${src_main})

