#/bin/bash
echo "building glfw"
mkdir -p vendor/glfw/build
cd vendor/glfw/build
cmake .. 
make -j$(cat /proc/cpuinfo | grep -c vendor_id)
