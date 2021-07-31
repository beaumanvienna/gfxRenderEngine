<p align="center">
  <img width="80" src="resources/atlas/images/I_ENGINE.png">
</p>

# gfxRenderEngine: A graphics rendering engine
<br />
<br />
Features: <br />
<br />
OpenGL, sprite sheets, sprite sheet animations, sprite sheet generator, <br />
Linux and Windows support, hotplug gamepad, toggle fullscreen (key F), <br />
debug imgui window (key I), camera movement via mouse scroll wheel and<br />
dpad/trigger buttons on the controller (use key R to reset view), shaders,<br />
tetragon player walk areas, animation sequences, render API abstraction, <br />
clear engine/application separation, event system,  batch rendering, GUI<br />
<br />
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

### Windows Build Instructions<br />
<br />
Dependencies: premake5, VS2019<br />
In a terminal, starting from the root folder gfxRenderEngine:<br />
premake5.exe vs2019<br />
<br />
Open the solution for gfxRenderEngine in VS2019 and hit F5<br />
To build the spritesheet generator, set it as startup project in the gfxRenderEngine solution and hit F5<br />
<br />

