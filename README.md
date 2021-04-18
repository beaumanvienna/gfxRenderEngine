<p align="center">
  <img width="80" src="resources/pictures/barrel.png">
</p>

# gfxRenderEngine: A graphics rendering engine
<br />
<br />
Dependencies: premake5, gcc compiler, -lGL -ldl -lpthread (e.g. libgl1-mesa-dev, libdl, libpthread on Ubuntu)<br />
GLEW dependencies: see vendor/glew/README.md <br />
(e.g. `sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev libosmesa-dev` on Ubuntu)<br />
<br />
<br />
Install source code: <br />
git clone https://github.com/beaumanvienna/gfxRenderEngine<br />
cd gfxRenderEngine<br />
git submodule update --init --recursive<br />
<br />
<br />
Create project files for gcc: <br />
premake5 gmake2<br />
<br />
<br />
Compile: make

