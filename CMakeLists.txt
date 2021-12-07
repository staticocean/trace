cmake_minimum_required(VERSION 3.16)
project(trace)

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

add_compile_options(-march=native -pthread)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)

# PICOC Support
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-int-to-void-pointer-cast")

include_directories(./)
include_directories(./src/)
include_directories(./lib/)
include_directories(./lib/sftlstd/include/)
include_directories(./lib/sftltrc/include/)
include_directories(./lib/sftlgui/include/)

if (UNIX)

    file(GLOB src_main      ./src/* ./src/unix/*)

endif (UNIX)

if (WIN32)

    file(GLOB src_main      ./src/* ./src/win/*)
    file(GLOB src_fmemopen  ./lib/fmemopen/*)

endif (WIN32)

file(GLOB src_tinymat   ./lib/tinymat/*)
file(GLOB src_json11    ./lib/json11/*)

add_executable(trace ${src_main} ${src_fmemopen} ${src_tinymat} ${src_json11})

add_subdirectory(lib/sftlgui)
add_subdirectory(lib/sftlstd)
add_subdirectory(lib/sftltrc)
#add_subdirectory(lib/json11)
#add_subdirectory(lib/tinymat)

target_link_libraries(trace LINK_PUBLIC sftlgui)
target_link_libraries(trace LINK_PUBLIC sftlstd)
target_link_libraries(trace LINK_PUBLIC sftltrc)

target_link_libraries(trace LINK_PUBLIC fmemopen)
#target_link_libraries(trace LINK_PUBLIC json11)
#target_link_libraries(trace LINK_PUBLIC tinymat)

