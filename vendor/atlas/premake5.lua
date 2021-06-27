-- premake5.lua

project "SpriteSheetGenerator"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
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

    flags
    {
        "MultiProcessorCompile"
    }
    
    filter "system:linux"
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

        
    filter "system:windows"
        links
        {
        }
        libdirs 
        {
        }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"


