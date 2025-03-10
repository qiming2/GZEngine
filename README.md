# GZEngine
GZ Game Engine

## Build

Install VulkanSDK yourself, fallback will use lib in the vendor which might not work
For debug version, it uses validation layer which is not included in the libs, so you have to
download VulkanSDK to run in debug mode, and make sure the appropriate env is set

git clone repo

git submodule init

git submodule update

### macOS

./build.sh

### window

build.bat

### linux

not supported yet


generated projects, files, etc will live in build folder