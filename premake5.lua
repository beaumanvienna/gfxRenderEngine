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
    targetdir "bin/%{cfg.buildcfg}"

    files 
    { 
        "engine/**.h", 
        "engine/**.cpp",
        "application/**.h", 
        "application/**.cpp",
        "vendor/glfw/**.h", 
        "vendor/glfw/**.cpp"
    }

    includedirs 
    { 
        "engine/include", 
        "vendor/imgui",
        "vendor/glfw/include",
        "engine/platform",
        "/usr/include/SDL2" 
    }
    
    libdirs 
    {
        "vendor/glfw/build/src" 
    }
    
    links
    {
        "glfw3",
        "GL",
        "dl",
        "pthread"
    }
    
    prebuildcommands
    {
        "scripts/build_glfw.sh"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"


