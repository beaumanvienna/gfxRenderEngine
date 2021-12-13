<p align="center">
  <img width="80" src="resources/atlas/images/I_ENGINE.png">
</p>

# gfxRenderEngine: A graphics rendering engine
<br/>
<br/>

## Features<br/>
OpenGL, sprite sheets, sprite sheet animations, sprite sheet generator, 
Linux and Windows support, gamepad hotplug support, debug imgui window (key m), 
camera movement, shaders,
tetragon player walk areas, animation sequences, render API abstraction, 
engine/application separation, event system,  layer system, batch rendering, 
a GUI including its own layer system and event system,
settings manager based on yaml, resource system, zero install,
controller setup, framebuffers, tilemap support<br/>
<br/>
Download for Windows: [marleyInstaller.exe](https://drive.google.com/file/d/1olewZdPN6LnjpOLrhj7hZaEmMlWjWq0G/view?usp=sharing)<br>
<br>
Demo video (Linux): https://streamable.com/ufej14 <br/>
Demo video (Visual Studio): https://streamable.com/w7japb

## History<br/>
gfxRenerEngine was inspired by two Youtube series, The Cherno's Game Engine series
and his OpenGL series. The code is partially based on The Cherno's Hazel engine. The 
GUI used in gfxRenderEngine was ported over from PPSSPP and extended. The example 
application and controller support are based on the emulator bundle project Marley 
(https://github.com/beaumanvienna/marley). <br/>
<br/>
Sprite sheet support was originally based on PPSSP code, however, the sprite sheet generator is 
now based on the tutorial at https://glusoft.com/tutorials/sfml/sprite-sheet-generator.
The mascot is "borrowed" from Moneky Island (and should one day be replaced by an open-source
character). The libraries in the vendor folder are open-source projects with individual licenses.
The resource system and the settings system, as well as tilemap support, animation sequences,
character movement, etc. were written for this project.<br/>

## Installation and Build Instructions<br/>
<br/>
Install the source code: <br/>
git clone --recurse-submodules https://github.com/beaumanvienna/gfxRenderEngine<br/>
cd gfxRenderEngine<br/>
<br/>

### Linux Build Instructions<br/>
<br/>
Ubuntu:<br/>
<br/>
sudo add-apt-repository ppa:beauman/marley<br/>
sudo add-apt-repository universe<br/>
sudo apt-get update<br/>
sudo apt install premake5 git build-essential cmake xorg-dev libudev-dev libxrandr-dev libasound2-dev \<br/>
libjack-dev libglu1-mesa-dev libglib2.0-dev libsndfile1-dev libsdl2-mixer-dev<br/>
<br/>
Create project files for gcc: <br/>
premake5 gmake2<br/>
<br/>
<br />
#define the number of CPU cores to be used for compiling<br />
#e.g. "-j4" <br />
#To use all available CPU cores, say:<br />
export MAKEFLAGS=-j$(nproc)<br />
<br />
<br />
Compile and run debug target: make verbose=1 && ./bin/Debug/engine <br/>
Compile and run release target: make config=release verbose=1 && ./bin/Release/engine<br/>
<br/>
Make clean: make clean && rm -rf vendor/glfw/build && cd vendor/glew && make clean && cd ../..<br/>
<br/>

### Windows Build Instructions<br/>
<br/>
Dependencies: premake5, VS2019<br/>
In a terminal, starting from the root folder gfxRenderEngine:<br/>
premake5.exe vs2019<br/>
<br/>
Open the solution for gfxRenderEngine, switch to Release, and hit F5<br/>
<br/>

