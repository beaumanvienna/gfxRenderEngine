-- premake5.lua


project "SpriteSheetGenerator"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin/objectFiles")
    
    defines
    {
        "GENERATOR_VERSION=\"0.0.1\"",
        "SFML_STATIC" 
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
        includedirs 
        { 
            "../sfml/include/"
        }
        libdirs 
        {
            "../sfml/build/lib/debug"
        }
        links
        {
            "sfml-graphics-s-d",
            "sfml-window-s-d",
            "sfml-system-s-d",
            "sfml-audio-s-d",
            "sfml-network-s-d",
            "opengl32",
            "winmm",
            "gdi32",
            "ws2_32"
        }

    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"


