-- premake5.lua
workspace "gfxRenderEngine"
    architecture "x86_64"
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
        "ENGINE_VERSION=\"0.1.0\"",
        "GLEW_STATIC",
        "SDL_MAIN_HANDLED"
    }

    files 
    { 
        "engine/**.h", 
        "engine/**.cpp",
        "application/**.h", 
        "application/**.cpp",
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
        "engine/auxiliary",
        "engine/transform",
        "vendor/imgui",
        "vendor/glfw/include",
        "vendor/glew/include",
        "vendor/stb",
        "vendor/glm",
        "vendor/sdl/include",
        "vendor/imgui/backends",
        "resources/atlas",
        "vendor/spdlog/include",
        "application",
        "application/characters",
        "application/UI",
        "application/splash"
    }
    
    libdirs 
    {
        "vendor/glfw/build/src"
    }

    flags
    {
        "MultiProcessorCompile"
    }
    
    filter "system:linux"
        prebuildcommands
        {
            "scripts/build_sdl.sh",
            "scripts/build_sfml.sh"
        }
        links
        {
            "glfw3",
            "SDL2",
            "GL",
            "dl",
            "pthread"
        }
        libdirs 
        {
            "vendor/glew/lib",
            "vendor/sdl/build/.libs"
        }
        
    filter "system:windows"
        links
        {
            "glfw3",
            "SDL2",
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
            "Setupapi"
        }
        libdirs 
        {
            "vendor/glew/build/src",
            "vendor/sdl/build/%{cfg.buildcfg}"
        }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"


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
        staticruntime "On"

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
        staticruntime "On"

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

include "vendor/atlas"
