#/bin/bash
echo "building sdl"
cd vendor/sdl
git update-index --assume-unchanged include/SDL_config.h include/SDL_revision.h
./configure
make -j$(cat /proc/cpuinfo | grep -c vendor_id)
