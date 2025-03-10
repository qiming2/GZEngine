@echo off

glslc.exe GZEditor/asset/shader/basic.vert -o GZEditor/asset/shader/basic_vert.spv
glslc.exe GZEditor/asset/shader/basic.frag -o GZEditor/asset/shader/basic_frag.spv
pause