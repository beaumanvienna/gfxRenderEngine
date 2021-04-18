#/bin/bash
echo "building glfw"
mkdir -p vendor/glfw/build
cd vendor/glfw/build
cmake .. 
make
