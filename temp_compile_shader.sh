#usr/bin/zsh!

echo "Compiling shader..."
glslc GZEditor/asset/shader/basic.vert -o GZEditor/asset/shader/basic_vert.spv
glslc GZEditor/asset/shader/basic.frag -o GZEditor/asset/shader/basic_frag.spv