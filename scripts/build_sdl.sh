#/bin/bash
echo "building sdl"
cd vendor/sdl
./configure
make -j$(cat /proc/cpuinfo | grep -c vendor_id)
