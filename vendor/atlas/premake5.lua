-- premake5.lua
workspace "gfxRenderEngine"
    architecture "x86_64"
    configurations 
    { 
        "Debug", 
        "Release" 
    }

project "SpriteSheetGenerator"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    
    defines
    {
        "GENERATOR_VERSION=\"0.0.1\"" 
    }

    files 
    { 
        "*.h", 
        "*.cpp"
    }

    includedirs 
    { 
        "../sfml/include/SFML/"
    }
    
    libdirs 
    {
        "../sfml/build/lib"
    }
    
    links
    {
        "dl",
        "sfml-graphics",
        "sfml-window",
        "sfml-system",
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


