
-- Team Engine 2021
-- License: https://github.com/beaumanvienna/gfxRenderEngine/blob/master/LICENSE

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
            "../sfml/build/lib"
        }
        links
        {
            "sfml",
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


