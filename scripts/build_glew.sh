#/bin/bash
echo "building glew"
cd vendor/glew
make -j$(cat /proc/cpuinfo | grep -c vendor_id)
