# GZEngine
GZ Game Engine

## Build

Install VulkanSDK yourself, fallback will use lib in the vendor which might not work
For debug version, it uses validation layer which is not included in the libs, so you have to
download VulkanSDK to run in debug mode, and make sure the appropriate env is set

!!! OnMacOS: need to opt install into global system, otherwise app can't find the right MTV lib

git clone repo

git submodule init

git submodule update

If you have not, download vulkan sdk:
https://vulkan.lunarg.com/

Download cmake: minimum currently 3.16, but higher is better, some packages use 3.5 and above(currently it does not cause any issue)

### macOS

./temp_compile_shader.sh
./build.sh

### window

temp_compile_shader.bat
build.bat

### linux

not tested yet


generated projects, files, etc will live in build folder