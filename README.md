<p align="center">
  <img width="80" src="resources/pictures/barrel.png">
</p>

# gfxRenderEngine: A graphics rendering engine
<br />

## Installation and Build Instructions<br />
<br />
Install the source code: <br />
git clone https://github.com/beaumanvienna/gfxRenderEngine<br />
cd gfxRenderEngine<br />
git submodule update --init --recursive<br />
<br />
### Linux Build Instructions<br />
<br />
Dependencies: premake5, gcc compiler, -lGL -ldl -lpthread (e.g. libgl1-mesa-dev, libdl, libpthread on Ubuntu)<br />
GLEW dependencies: see vendor/glew/README.md <br />
(e.g. `sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev libosmesa-dev` on Ubuntu)<br />
<br />
Create project files for gcc: <br />
premake5 gmake2<br />
<br />
Compile and run debug target: make verbose=1 && ./bin/Debug/engine <br />
Compile and run release target: make config=release verbose=1 && ./bin/Release/engine<br />
<br />
Make clean: make clean && rm -rf vendor/glfw/build && cd vendor/glew && make clean && cd ../..<br />
<br />

