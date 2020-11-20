cmake_minimum_required(VERSION 3.16)
project(trj)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wundefined-internal")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wdeprecated-declarations")

include_directories(./)
include_directories(lib/imgui/)
include_directories(lib/opengl/)
include_directories(lib/fmt/)
include_directories(lib/trj/)
include_directories(lib/picoc/)
include_directories(lib/nlohmann/)

file(GLOB src_main      "src/trj_opengl.cpp" "src/picoc*.*" "src/trj_gui*.*" "src/imgui_custom.*")
file(GLOB src_imgui     "lib/imgui/*.*")
file(GLOB src_opengl    "lib/opengl/*.*")
file(GLOB src_picoc     "lib/picoc/*.*" "lib/picoc/cstdlib/*.*")
file(GLOB src_imguizmo  "lib/imguizmo/*.*")
file(GLOB src_tinymat   "lib/tinymat/*.*")
file(GLOB src_fmemopen  "lib/fmemopen/*.*")

list(REMOVE_ITEM ${src_main}  "src/trj_webgl.*")
list(REMOVE_ITEM ${src_picoc} "lib/picoc/cstdlib/unistd.c")
list(REMOVE_ITEM ${src_picoc} "lib/picoc/platform/library_unix.c")
list(REMOVE_ITEM ${src_picoc} "lib/picoc/platform/platform_unix.c")

add_executable(trj ${src_main} ${src_imgui} ${src_opengl} ${src_fmt} ${src_picoc} ${src_imguizmo} ${src_tinymat} ${src_fmemopen})

find_library(lib_opengl32 opengl32)
target_link_libraries(trj LINK_PUBLIC ${lib_opengl32})
message( ${lib_opengl32} )

find_library(lib_glfw3_x64 glfw3_x64 lib/opengl/static/)
target_link_libraries(trj LINK_PUBLIC ${lib_glfw3_x64})
message( ${lib_glfw3_x64} )

