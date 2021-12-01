cmake_minimum_required(VERSION 3.16)
project(trace)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-security")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-int-to-void-pointer-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

# PICOC Support
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-int-to-void-pointer-cast")

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)

include_directories(lib/)
include_directories(src/)

if (UNIX)

    file(GLOB src_main      src/* src/unix/*)

endif (UNIX)

if (WIN32)

    file(GLOB src_main      src/* src/win/*)

endif (WIN32)

file(GLOB src_picoc     ./lib/picoc/* ./lib/picoc/cstdlib/*)

list(REMOVE_ITEM ${src_picoc} lib/picoc/cstdlib/unistd.c)
list(REMOVE_ITEM ${src_picoc} lib/picoc/platform/library_unix.c)
list(REMOVE_ITEM ${src_picoc} lib/picoc/platform/platform_unix.c)

add_executable(trajectory ${src_main} ${src_picoc})

add_subdirectory(lib/sftlgui)
add_subdirectory(lib/fmemopen)
add_subdirectory(lib/json11)
add_subdirectory(lib/tinymat)

target_link_libraries(trace LINK_PUBLIC sftlgui)
target_link_libraries(trace LINK_PUBLIC fmemopen)
target_link_libraries(trace LINK_PUBLIC json11)
target_link_libraries(trace LINK_PUBLIC tinymat)

