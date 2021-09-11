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
        "GLEW_STATIC",
        "SDL_MAIN_HANDLED"
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

        linkoptions { "-fno-pie -no-pie" }

        prebuildcommands
        {
            "scripts/build_sdl.sh",
            --"scripts/build_mednafen.sh",
            "scripts/build_sfml.sh"
        }
        files 
        { 
            "resources/linuxEmbeddedResources.cpp"
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
            "SDL2",
            "SDL2_image",
            "SDL2_ttf",
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
            --"emulators/mednafen/build/%{cfg.buildcfg}"
            "emulators/mednafen/build/src/"
        }
        defines
        {
            "LINUX",
        }
    
    filter "system:windows"
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
            "z",
            "Dsound",
            "pthread",
            "sndfile"
        }
        libdirs 
        {
            "vendor/glew/build/src",
            "vendor/sdl/build/%{cfg.buildcfg}",
            "vendor/sdl_mixer/build/%{cfg.buildcfg}",
            "vendor/sndfile/build/%{cfg.buildcfg}",
            "vendor/iconv/build/%{cfg.buildcfg}",
            "vendor/win/pthread/build/%{cfg.buildcfg}",
            "emulators/mednafen/build/%{cfg.buildcfg}"
            --"emulators/mednafen/build/mednafen/"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter { "action:gmake*" }
        buildoptions { "-fdiagnostics-color=always" }
    

project "yaml-cpp"
    kind "StaticLib"
    language "C++"

    targetdir ("vendor/yaml-cpp/build")
    objdir ("vendor/yaml-cpp/build")

    files
    {
        "vendor/yaml-cpp/src/**.h",
        "vendor/yaml-cpp/src/**.cpp",
        "vendor/yaml-cpp/include/**.h"
    }

    includedirs
    {
        "vendor/yaml-cpp/include"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"

    filter "system:linux"
        pic "On"
        systemversion "latest"
        cppdialect "C++17"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

project "glfw3"
    kind "StaticLib"
    language "C"

    targetdir ("vendor/glfw/build/src")
    objdir ("vendor/glfw/build/objectFiles")

    files
    {
        "vendor/glfw/include/GLFW/glfw3.h",
        "vendor/glfw/include/GLFW/glfw3native.h",
        "vendor/glfw/src/glfw_config.h",
        "vendor/glfw/src/context.c",
        "vendor/glfw/src/init.c",
        "vendor/glfw/src/input.c",
        "vendor/glfw/src/monitor.c",
        "vendor/glfw/src/vulkan.c",
        "vendor/glfw/src/window.c"
    }
    filter "system:linux"
        pic "On"

        systemversion "latest"

        files
        {
            "vendor/glfw/src/x11_init.c",
            "vendor/glfw/src/x11_monitor.c",
            "vendor/glfw/src/x11_window.c",
            "vendor/glfw/src/xkb_unicode.c",
            "vendor/glfw/src/posix_time.c",
            "vendor/glfw/src/posix_thread.c",
            "vendor/glfw/src/glx_context.c",
            "vendor/glfw/src/egl_context.c",
            "vendor/glfw/src/osmesa_context.c",
            "vendor/glfw/src/linux_joystick.c"
        }

        defines
        {
            "_GLFW_X11"
        }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "vendor/glfw/src/win32_init.c",
            "vendor/glfw/src/win32_joystick.c",
            "vendor/glfw/src/win32_monitor.c",
            "vendor/glfw/src/win32_time.c",
            "vendor/glfw/src/win32_thread.c",
            "vendor/glfw/src/win32_window.c",
            "vendor/glfw/src/wgl_context.c",
            "vendor/glfw/src/egl_context.c",
            "vendor/glfw/src/osmesa_context.c"
        }

        defines 
        { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    if os.host() == "linux" then
       include "emulators/mednafen/mednafen.lua"
    end

    if os.host() == "windows" then
       include "vendor/SDL2.lua"
       include "vendor/SDL_mixer.lua"
       include "vendor/SFML.lua"
       include "vendor/sndfile.lua"
       include "vendor/zlib.lua"
       include "vendor/iconv.lua"
       include "emulators/mednafen/mednafen.lua"
       include "vendor/win/pthread.lua"
    end

    if os.host() == "linux" then
        project "resource-system-linux"
                    kind "StaticLib"
                        os.execute("glib-compile-resources resources/linuxEmbeddedResources.xml --target=resources/linuxEmbeddedResources.cpp --sourcedir=resources/ --generate-source")
                        os.execute("glib-compile-resources resources/linuxEmbeddedResources.xml --target=resources/linuxEmbeddedResources.h   --sourcedir=resources/ --generate-header")
    end

    include "vendor/atlas"
