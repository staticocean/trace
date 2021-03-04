cmake_minimum_required(VERSION 3.16)
project(trajectory)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-security")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-int-to-void-pointer-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

# PICOC Support
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-int-to-void-pointer-cast")

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)

include_directories(.)
include_directories(./src)
include_directories(./softael_lib)
include_directories(./lib/imgui)
include_directories(./lib/opengl)
include_directories(./lib/trj)
include_directories(./lib/picoc)
include_directories(./lib/json11)
include_directories(./lib/implot)
include_directories(./lib/clip)
include_directories(./lib/nfde)

file(GLOB src_main      "./src/*" "./src/win/*")
file(GLOB src_imgui     "./lib/imgui/*")
file(GLOB src_opengl    "./lib/opengl/*")
file(GLOB src_picoc     "./lib/picoc/*" "./lib/picoc/cstdlib/*.*")
file(GLOB src_tinymat   "./lib/tinymat/*")
file(GLOB src_fmemopen  "./lib/fmemopen/*")
file(GLOB src_json11    "./lib/json11/*")
file(GLOB src_implot    "./lib/implot/*")
file(GLOB src_clip      "./lib/clip/clip.cpp" "./lib/clip/image.cpp" "./lib/clip/clip_win.cpp")
file(GLOB src_nfde      "./lib/nfde/nfd_win.cpp")

list(REMOVE_ITEM ${src_picoc} "lib/picoc/cstdlib/unistd.c")
list(REMOVE_ITEM ${src_picoc} "lib/picoc/platform/library_unix.c")
list(REMOVE_ITEM ${src_picoc} "lib/picoc/platform/platform_unix.c")

add_executable(trajectory ${src_main} ${src_imgui} ${src_opengl} ${src_picoc} ${src_json11} ${src_tinymat} ${src_fmemopen} ${src_implot} ${src_clip} ${src_nfde})

target_link_libraries(trajectory LINK_PUBLIC shlwapi)
target_link_libraries(trajectory LINK_PUBLIC opengl32)
target_link_libraries(trajectory LINK_PUBLIC ../lib/opengl/static/glfw3_x64)

