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
    
    defines
    {
        "ENGINE_VERSION=\"0.0.1\"" 
    }

    files 
    { 
        "engine/**.h", 
        "engine/**.cpp",
        "application/**.h", 
        "application/**.cpp",
        "vendor/glfw/**.h", 
        "vendor/glfw/**.cpp",
        "vendor/stb/**.cpp"
    }

    includedirs 
    { 
        "engine/include", 
        "vendor/imgui",
        "vendor/glfw/include",
        "vendor/glew/include",
        "engine/platform",
        "vendor/stb",
        "/usr/include/SDL2" 
    }
    
    libdirs 
    {
        "vendor/glfw/build/src",
        "vendor/glew/lib"
    }
    
    links
    {
        "glfw3",
        "GLEW",
        "GL",
        "dl",
        "pthread"
    }
    
    prebuildcommands
    {
        "scripts/build_glfw.sh",
        "scripts/build_glew.sh"
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


