-- premake5.lua
workspace "gfxRenderEngine"
    architecture "x86_64"
    startproject "engine"
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
        "ENGINE_VERSION=\"0.2.2\"",
        "GLEW_STATIC",
        "SDL_MAIN_HANDLED"
    }

    files 
    { 
        "engine/**.h", 
        "engine/**.cpp",
        "application/**.h", 
        "application/**.cpp",
        "resources/resources.cpp",
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
        "engine/sound",
        "engine/auxiliary",
        "engine/transform",
        "engine/settings",
        "engine/UI",
        "engine/UI/Common/",
        "engine/UI/Common/Data/Text/",
        "engine/UI/Common/UI",
        "engine/UI/Common/Input",
        "engine/UI/Common/Math",
        "engine/UI/Common/Render",
        "engine/UI/Common/File",
        "engine/UI/Common/Thread",
        "engine/UI/Render",
        "vendor/yaml-cpp/include",
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
        "application/splash",
        "application/UI",
        "application/testing",
        "resources"
    }
    
    libdirs
    {
        "vendor/glfw/build/src",
        "vendor/yaml-cpp/build"
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
        files 
        { 
            "resources/linuxEmbeddedResources.cpp"
        }
        includedirs 
        { 
            "resources",
            "/usr/include/glib-2.0",
            "/usr/lib/x86_64-linux-gnu/glib-2.0/include"
        }
        links
        {
            "glfw3",
            "SDL2",
            "GL",
            "dl",
            "pthread",
            "yaml-cpp",
            "gio-2.0",
            "glib-2.0"
        }
        libdirs 
        {
            "vendor/glew/lib",
            "vendor/sdl/build/.libs"
        }
        defines
        {
            "LINUX",
        }
        
    filter "system:windows"
        files 
        {
            "resources/windowsEmbeddedResources.rc"
        }
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
            "Setupapi",
            "yaml-cpp"
        }
        libdirs 
        {
            "vendor/glew/build/src",
            "vendor/sdl/build/%{cfg.buildcfg}"
        }
        defines
        {
            "WINDOWS",
        }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
    
project "yaml-cpp"
    kind "StaticLib"
    language "C++"
    
    targetdir ("vendor/yaml-cpp/build")
    objdir ("vendor/yaml-cpp/build")

    files
    {
        "vendor/yaml-cpp/src/**.h",
        "vendor/yaml-cpp/src/**.cpp",
        "vendor/yaml-cpp/include/**.h"
    }

    includedirs
    {
        "vendor/yaml-cpp/include"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"

    filter "system:linux"
        pic "On"
        systemversion "latest"
        cppdialect "C++17"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

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

project "resource-system-linux"
    kind "StaticLib"
    filter "system:linux"
        os.execute("glib-compile-resources resources/linuxEmbeddedResources.xml --target=resources/linuxEmbeddedResources.cpp --sourcedir=resources/ --generate-source")
        os.execute("glib-compile-resources resources/linuxEmbeddedResources.xml --target=resources/linuxEmbeddedResources.h   --sourcedir=resources/ --generate-header")


project "SDL2"
    filter "system:windows"               
        kind          "StaticLib"         
        staticruntime "off"

    filter "system:macosx"
        kind          "StaticLib"
        staticruntime "on"

    filter "system:linux"
        kind          "StaticLib"
        staticruntime "on"
    filter {}

    language          "C++"              
    cppdialect        "C++17"            
                            
    systemversion     "latest"

    flags {
                      "NoRuntimeChecks", 
                      "NoBufferSecurityCheck"
    }

    vectorextensions  "SSE"              
    
    targetdir         "vendor/sdl/build/%{cfg.buildcfg}"
    objdir            "vendor/sdl/build/%{cfg.buildcfg}/%{prj.name}"

    includedirs      {"vendor/sdl/include"}

    filter "system:windows"
        links {
                     "setupapi",
                     "winmm",
                     "imm32",
                     "version",
        }
    filter {}

    files {
        -- All platforms.
        -- Header files.                                    -- C files.
        "vendor/sdl/include/*.h",                                        
                                                            
        "vendor/sdl/src/audio/disk/*.h",                               "vendor/sdl/src/atomic/*.c",
        "vendor/sdl/src/audio/dummy/*.h",                              "vendor/sdl/src/audio/disk/*.c",
        "vendor/sdl/src/audio/*.h",                                    "vendor/sdl/src/audio/dummy/*.c",
        "vendor/sdl/src/dynapi/*.h",                                   "vendor/sdl/src/audio/*.c",
        "vendor/sdl/src/events/blank_cursor.h",                        "vendor/sdl/src/cpuinfo/*.c",
        "vendor/sdl/src/events/default_cursor.h",                      "vendor/sdl/src/dynapi/*.c",
        "vendor/sdl/src/events/SDL_clipboardevents_c.h",               "vendor/sdl/src/events/*.c",
        "vendor/sdl/src/events/SDL_displayevents_c.h",                 "vendor/sdl/src/file/*.c",
        "vendor/sdl/src/events/SDL_dropevents_c.h",                    "vendor/sdl/src/haptic/*.c",
        "vendor/sdl/src/events/SDL_events_c.h",                        "vendor/sdl/src/joystick/hidapi/*.c",
        "vendor/sdl/src/events/SDL_gesture_c.h",                       "vendor/sdl/src/joystick/*.c",
        "vendor/sdl/src/events/SDL_keyboard_c.h",                      "vendor/sdl/src/libm/*.c",
        "vendor/sdl/src/events/SDL_mouse_c.h",                         "vendor/sdl/src/power/*.c",
        "vendor/sdl/src/events/SDL_sysevents.h",                       "vendor/sdl/src/render/opengl/*.c",
        "vendor/sdl/src/events/SDL_touch_c.h",                         "vendor/sdl/src/render/opengles2/*.c",
        "vendor/sdl/src/events/SDL_windowevents_c.h",                  "vendor/sdl/src/render/SDL_render.c",
        "vendor/sdl/src/haptic/SDL_syshaptic.h",                       "vendor/sdl/src/render/SDL_yuv_sw.c",
        "vendor/sdl/src/joystick/hidapi/*.h",                          "vendor/sdl/src/render/software/*.c",
        --[["src/joystick/hidapi/SDL_hidapijoystick_c.h",]]            "vendor/sdl/src/*.c",
        "vendor/sdlsrc/joystick/SDL_hidapijoystick_c.h",               "vendor/sdl/src/sensor/dummy/SDL_dummysensor.c",
        "vendor/sdlsrc/joystick/SDL_joystick_c.h",                     "vendor/sdl/src/sensor/SDL_sensor.c",
        "vendor/sdlsrc/joystick/SDL_sysjoystick.h",                    "vendor/sdl/src/stdlib/*.c",
        "vendor/sdlsrc/libm/*.h",                                      "vendor/sdl/src/thread/generic/SDL_syscond.c",
        "vendor/sdlsrc/render/opengl/*.h",                             "vendor/sdl/src/thread/*.c",
        "vendor/sdlsrc/render/opengles/*.h",                           "vendor/sdl/src/thread/windows/SDL_sysmutex.c",
        "vendor/sdlsrc/render/SDL_yuv_sw_c.h",                         "vendor/sdl/src/thread/windows/SDL_syssem.c",
        "vendor/sdlsrc/render/software/*.h",                           "vendor/sdl/src/thread/windows/SDL_systhread.c",
        "vendor/sdlsrc/render/SDL_sysrender.h",                        "vendor/sdl/src/thread/windows/SDL_systls.c",
        "vendor/sdlsrc/SDL_dataqueue.h",                               "vendor/sdl/src/timer/*.c",
        "vendor/sdlsrc/SDL_error_c.h",                                 "vendor/sdl/src/timer/windows/SDL_systimer.c",
        "vendor/sdlsrc/sensor/dummy/*.h",                              "vendor/sdl/src/video/dummy/*.c",
        "vendor/sdlsrc/sensor/*.h",                                    "vendor/sdl/src/video/*.c",
        "vendor/sdlsrc/thread/*.h",                                    "vendor/sdl/src/video/yuv2rgb/*.c",
        "vendor/sdlsrc/timer/*.h",
        "vendor/sdlsrc/video/dummy/*.h",
        "vendor/sdlsrc/video/*.h",
    }

    filter "system:windows"
        files {
            -- Windows specific files.
            -- Header files.                                            -- C files.
                                                                       "vendor/sdl/src/main/windows/*.c",
            "vendor/sdl/include/SDL_config_windows.h",                    
            "vendor/sdl/src/audio/directsound/*.h",                    "vendor/sdl/src/audio/directsound/*.c",
            "vendor/sdl/src/audio/wasapi/*.h",                         "vendor/sdl/src/audio/winmm/*.c",
            "vendor/sdl/src/audio/winmm/*.h",                          "vendor/sdl/src/audio/wasapi/*.c",
            --[["src/windows/SDL_directx.h",]]                         "vendor/sdl/src/core/windows/*.c",
            "vendor/sdl/src/core/windows/*.h",                         "vendor/sdl/src/filesystem/windows/*.c",
            "vendor/sdl/src/haptic/windows/*.h",                       "vendor/sdl/src/haptic/windows/*.c",
            "vendor/sdl/src/joystick/windows/*.h",                     "vendor/sdl/src/joystick/windows/*.c",
            "vendor/sdl/src/render/direct3d11/SDL_shaders_d3d11.h",    "vendor/sdl/src/hidapi/windows/*.c",
            "vendor/sdl/src/render/direct3d/*.h",                      "vendor/sdl/src/loadso/windows/*.c",
            "vendor/sdl/src/render/SDL_d3dmath.h",                     "vendor/sdl/src/power/windows/*.c",
            "vendor/sdl/src/thread/windows/*.h",                       "vendor/sdl/src/render/direct3d11/*.c",
            "vendor/sdl/src/video/windows/SDL_vkeys.h",                "vendor/sdl/src/render/direct3d/*.c",
            "vendor/sdl/src/video/windows/SDL_windowsclipboard.h",     "vendor/sdl/src/render/SDL_d3dmath.c",
            "vendor/sdl/src/video/windows/SDL_windowsevents.h",        "vendor/sdl/src/video/windows/*.c",
            "vendor/sdl/src/video/windows/SDL_windowsframebuffer.h",
            "vendor/sdl/src/video/windows/SDL_windowskeyboard.h",
            "vendor/sdl/src/video/windows/SDL_windowsmessagebox.h",
            "vendor/sdl/src/video/windows/SDL_windowsmodes.h",
            "vendor/sdl/src/video/windows/SDL_windowsmouse.h",
            "vendor/sdl/src/video/windows/SDL_windowsopengl.h",
            "vendor/sdl/src/video/windows/SDL_windowsshape.h",
            "vendor/sdl/src/video/windows/SDL_windowsvideo.h",
            "vendor/sdl/src/video/windows/SDL_windowsvulkan.h",
            "vendor/sdl/src/video/windows/SDL_windowswindow.h",
            "vendor/sdl/src/video/windows/wmmsg.h",
        }

        
    inlining          "Explicit"             -- General optimization options.
    intrinsics        "Off"

    filter "system:windows"
        systemversion "latest"
        defines {
                      "_WINDOWS"
        }

    filter "configurations:Debug"
        defines {
                      "_DEBUG"
        }
        runtime       "Debug"
        symbols       "On"

    filter "configurations:Release"
        defines {
                      "NDEBUG"
        }
        runtime       "Release"
        optimize      "Speed"


    filter "configurations:Development"     -- These are the configurations I tend to
        defines {                           -- use in my projects, but I have added 
                      "NDEBUG"              -- the default ones anyway.
        }
        runtime       "Release"
        optimize      "On"

    filter "configurations:Ship"
        defines {
                      "NDEBUG"
        }
        runtime       "Release"
        optimize      "Speed"
