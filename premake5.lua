-- premake5.lua
workspace "gfxRenderEngine"
    architecture "x86_64"
    startproject "engine"
    configurations 
    { 
        "Debug", 
        "Release" 
    }

project "engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"

    defines
    {
        "ENGINE_VERSION=\"0.2.2\"",
        "MULTI_APP",
        "GLEW_STATIC"
    }

    files 
    { 
        "engine/**.h", 
        "engine/**.cpp",
        "application/**.h", 
        "application/**.cpp",
        "resources/resources.cpp",
        "vendor/glfw/**.h", 
        "vendor/glfw/**.cpp",
        "vendor/stb/**.cpp",
        "vendor/imgui/backends/imgui_impl_glfw.cpp",
        "vendor/imgui/backends/imgui_impl_opengl3.cpp",
        "vendor/imgui/*.cpp",
        "vendor/glew/src/glew.c",
        "vendor/glew/include/GL/**"
    }

    includedirs 
    { 
        "engine",
        "engine/events",
        "engine/imgui",
        "engine/log",
        "engine/platform/",
        "engine/platform/OpenGL",
        "engine/platform/SDL",
        "engine/renderer",
        "engine/animation",
        "engine/spritesheet",
        "engine/controller",
        "engine/layer",
        "engine/shader",
        "engine/sound",
        "engine/auxiliary",
        "engine/transform",
        "engine/settings",
        "engine/UI",
        "engine/UI/Common/",
        "engine/UI/Common/Data/Text/",
        "engine/UI/Common/UI",
        "engine/UI/Common/Input",
        "engine/UI/Common/Math",
        "engine/UI/Common/Render",
        "engine/UI/Common/File",
        "engine/UI/Common/Thread",
        "engine/UI/Render",
        "vendor/yaml-cpp/include",
        "vendor/imgui",
        "vendor/glfw/include",
        "vendor/glew/include",
        "vendor/stb",
        "vendor/glm",
        "vendor/sdl/include",
        "vendor/sdl_mixer/include",
        "vendor/imgui/backends",
        "resources/atlas",
        "vendor/spdlog/include",
        "application",
        "resources"
    }

    libdirs
    {
        "vendor/glfw/build/src",
        "vendor/yaml-cpp/build"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    filter "system:linux"

        linkoptions { "-fno-pie -no-pie -lSDL2" }
        files 
        { 
            "resources/gnuEmbeddedResources.cpp"
        }
        includedirs 
        { 
            "resources",
            "/usr/include/glib-2.0",
            "/usr/lib/x86_64-linux-gnu/glib-2.0/include"
        }
        links
        {
            "mednafen_marley",
            "SDL2_mixer",
            "asound",
            "m",
            "dl", 
            "pthread",
            "jack",
            "sndfile",
            "z",
            "glfw3",
            "GL",
            "yaml-cpp",
            "gio-2.0",
            "glib-2.0"
        }
        libdirs
        {
            "vendor/glew/lib",
            "vendor/sdl/build/.libs",
            "vendor/sdl_mixer/build/.libs",
            "emulators/mednafen/build/%{cfg.buildcfg}"
        }
        defines
        {
            "LINUX",
        }
    
    filter "system:windows"
        defines
        {
            "SDL_MAIN_HANDLED"
        }
        files 
        {
            "resources/windowsEmbeddedResources.rc"
        }
        links
        {
            "mednafen_marley",
            "glfw3",
            "sdl2",
            "sdl2_main",
            "sdl_mixer",
            "libvorbis",
            "libogg",
            "OpenGL32",
            "winmm",
            "imagehlp", 
            "dinput8", 
            "dxguid", 
            "user32", 
            "gdi32", 
            "imm32", 
            "ole32",
            "oleaut32",
            "shell32",
            "version",
            "uuid",
            "Setupapi",
            "yaml-cpp",
            "iconv",
            "Dsound",
            "sndfile"
        }
        libdirs 
        {
            "vendor/glew/build/src",
            "vendor/sdl/build/%{cfg.buildcfg}",
            "vendor/sdl_mixer/build/%{cfg.buildcfg}",
            "vendor/zlib/build/%{cfg.buildcfg}"
        }

    filter { "system:windows", "action:gmake*"}
        prebuildcommands
        {
            "scripts/build_mednafen_win.sh"
        }
        buildoptions { "-fdiagnostics-color=always" }
        includedirs
        { 
            "/mingw64/include/glib-2.0",
            "/mingw64/lib/glib-2.0/include"
        }
        files 
        { 
            "resources/gnuEmbeddedResources.cpp"
        }
        libdirs 
        {
            "emulators/mednafen/build/mednafen/"
        }
        links
        {
            "z",
            "glib-2.0",
            "gio-2.0"
        }

    filter { "system:windows", "action:vs*" }
        links{ "zlib" }
        libdirs 
        {
            "vendor/sndfile/build/%{cfg.buildcfg}",
            "vendor/iconv/build/%{cfg.buildcfg}",
            "emulators/mednafen/build/%{cfg.buildcfg}"
        }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"

    include "vendor/glfw.lua"
    include "vendor/yaml.lua"
    include "vendor/atlas"

    if os.host() == "linux" then

        include "emulators/mednafen/mednafen.lua"

    end

    if ( (os.host() == "linux") or (os.host() == "windows" and _ACTION == "gmake2") ) then

        project "resource-system-gnu"
            kind "StaticLib"
            os.execute("glib-compile-resources resources/gnuEmbeddedResources.xml --target=resources/gnuEmbeddedResources.cpp --sourcedir=resources/ --generate-source")
            os.execute("glib-compile-resources resources/gnuEmbeddedResources.xml --target=resources/gnuEmbeddedResources.h   --sourcedir=resources/ --generate-header")
    end


    if os.host() == "windows" then
        include "vendor/SDL2.lua"
        include "vendor/SDL_mixer.lua"
        include "vendor/SFML.lua"
        include "vendor/zlib.lua"
       
        if _ACTION == "vs2019" then
            include "vendor/sndfile.lua"
            include "vendor/iconv.lua"
            include "emulators/mednafen/mednafen.lua"
        end
    end
