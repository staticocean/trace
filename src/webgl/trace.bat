@echo on
cd %~dp0
call "C:\Users\cel\dev\git\public\emsdk\emsdk_env.bat"
@echo on

cmd /c emcc ^
    ../src/trj_gui.cpp ^
    ../src/gui_eng.cpp ^
    ../src/gui_traj.cpp ^
    ../src/gui_obj.cpp ^
    ../src/gui_menu.cpp ^
    ../src/trj_webgl.cpp ^
    ../lib/imgui/webgl/imgui_impl_glfw.cpp ^
    ../lib/imgui/webgl/imgui_impl_opengl3.cpp ^
    ../lib/imgui/imgui.cpp ^
    ../lib/imgui/imgui_draw.cpp ^
    ../lib/imgui/imgui_demo.cpp ^
    ../lib/imgui/imgui_widgets.cpp ^
    ../lib/trj/trj_ctrl.cpp^
    ../lib/trj/trj_eng.cpp^
    ../lib/trj/trj_obj.cpp^
    ../lib/trj/trj_proc.cpp^
    ../lib/trj/trj_types.cpp^
    ../lib/trj/trj_utils.cpp^
    ../lib/trj/trj_traj.cpp^
    ../lib/trj/trj_bz.cpp^
    ../lib/trj/vl.cpp^
    ../lib/trj/vl_svd.cpp^
    ../lib/picoc/picoc_clibrary.c^
    ../lib/picoc/picoc_config.c^
    ../lib/picoc/picoc_debug.c^
    ../lib/picoc/picoc_expression.c^
    ../lib/picoc/picoc_heap.c^
    ../lib/picoc/picoc_include.c^
    ../lib/picoc/picoc_lex.c^
    ../lib/picoc/picoc_parse.c^
    ../lib/picoc/picoc_platform.c^
    ../lib/picoc/picoc_table.c^
    ../lib/picoc/picoc_type.c^
    ../lib/picoc/picoc_variable.c^
    ../lib/picoc/cstdlib/ctype.c^
    ../lib/picoc/cstdlib/errno.c^
    ../lib/picoc/cstdlib/math.c^
    ../lib/picoc/cstdlib/stdbool.c^
    ../lib/picoc/cstdlib/stdio.c^
    ../lib/picoc/cstdlib/stdlib.c^
    ../lib/picoc/cstdlib/string.c^
    ../lib/picoc/cstdlib/time.c^
    -o ./trj_webgl.js -lGL -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 ^
    -I ./ ^
    -I ../ ^
    -I ../src/ ^
    -I ../lib/imgui/ ^
    -I ../lib/trj/ ^
    -I ../lib/fmt/ ^
    -I ../lib/picoc/ ^
    -g4 --preload-file ../res/fonts/ --source-map-base http://127.0.0.1:8000/src/

cd ../
python -m http.server --cgi -b 127.0.0.1