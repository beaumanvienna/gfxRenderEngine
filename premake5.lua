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
        "vendor/stb/**.cpp",
        "vendor/imgui/backends/imgui_impl_glfw.cpp",
        "vendor/imgui/backends/imgui_impl_opengl3.cpp",
        "vendor/imgui/*.cpp"
    }

    includedirs 
    { 
        "engine/include", 
        "vendor/imgui",
        "vendor/glfw/include",
        "vendor/glew/include",
        "engine/platform",
        "vendor/stb",
        "vendor/glm",
        "vendor/sdl/include",
        "vendor/imgui/backends",
        "resources/images/ui_atlas"
    }
    
    libdirs 
    {
        "vendor/glfw/build/src",
        "vendor/glew/lib",
        "vendor/sdl/build/.libs/"
    }
    
    links
    {
        "glfw3",
        "GLEW",
        "GL",
        "dl",
        "pthread",
        "SDL2"
    }
    
    prebuildcommands
    {
        "scripts/build_glfw.sh",
        "scripts/build_glew.sh",
        "scripts/build_sdl.sh",
        "scripts/build_sfml.sh",
        "build_atals_tool.sh"
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


