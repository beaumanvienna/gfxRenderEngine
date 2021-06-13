-- premake5.lua

project "SpriteSheetGenerator"
    kind "ConsoleApp"
    language "C++"
    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin/objectFiles")
    
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
        "vendor/sfml/include/SFML/"
    }
    
    libdirs 
    {
        "vendor/sfml/build/lib"
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


