-- premake5.lua
workspace "gfxRenderEngine"
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
        "application/**.cpp" 
    }

   includedirs 
   { 
        "engine/include", 
        "vendor/imgui",
        "vendor/glfw/include",
        "/usr/include/SDL2" 
    }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

    excludes 
    { 
        "vendor/imgui/backends/**.cpp",
        "vendor/imgui/examples/**.cpp"
    }
