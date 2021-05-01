/* Engine Copyright (c) 2021 Engine Development Team 
   https://github.com/beaumanvienna/gfxRenderEngine

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation files
   (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "engine.h"
#include "platform.h"
#include "core.h"
#include "log.h"
#include "imgui_engine.h"
#include "OpenGL/GL.h"
#include "stb_image.h"
#include "SDL.h"
// --- Class Engine ---

Engine::Engine(int argc, char** argv) :
            m_Running(false), m_Window(nullptr), m_WindowScale(0), m_ScaleImguiWidgets(0),
            m_WindowAspectRatio(0), m_WindowWidth(0), m_WindowHeight(0)
{
}

Engine::~Engine()
{
}

bool Engine::Start()
{
    m_Running = true;
    // init logger
    if (!Log::Init())
    {
        std::cout << "Could initialize logger" << std::endl;
        m_Running = false;
    }
    else
    {
        // init glfw
        if (!InitGLFW())
        {
            m_Running = false;
        }
        else
        {
            // create main window
            if (!CreateMainWindow())
            {
                m_Running = false;
            }
            else
            {
                // init glew
                if (!InitGLEW())
                {
                    m_Running = false;
                }
                else
                {
                    // init SDL
                    if (!InitSDL())
                    {
                        m_Running = false;
                    }
                    else
                    {
                        // init imgui
                        m_ScaleImguiWidgets = m_WindowScale * 1.4f; 
                        if (!ImguiInit(m_Window, m_ScaleImguiWidgets))
                        {
                            m_Running = false;
                        }
                    }
                }
            }
        }
    }
    
    if (!m_Running)
    {
        std::cout << "Couldn't start engine, aborting"  << std::endl;
    } 
    else
    {    
        std::cout << std::endl;
        std::string infoMessage = "Starting engine (gfxRenderEngine) v" ENGINE_VERSION;
        Log::GetLogger()->info(infoMessage);
        std::cout << std::endl;
    }    
    return m_Running;
}

bool Engine::Shutdown()
{
    m_Running = false;
    GLCall(glfwTerminate());
    return m_Running;
}

bool Engine::CreateMainWindow()
{
    bool ok = false;
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);
    
    m_WindowWidth = videoMode->width / 1.5f;
    m_WindowHeight = m_WindowWidth / 16 * 9;
    int monitorX, monitorY;
    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

    // make window invisible before it gets centered
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "Engine v" ENGINE_VERSION, NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        std::cout << "Failed to create main window" << std::endl;
    }
    else
    {
        // center window
        glfwSetWindowPos(m_Window,
                         monitorX + (videoMode->width - m_WindowWidth) / 2,
                         monitorY + (videoMode->height - m_WindowHeight) / 2);
        
        // make the centered window visible
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwShowWindow(m_Window);
        
        // create context
        char description[1024];
        glfwMakeContextCurrent(m_Window);
        if (glfwGetError((const char**)(&description)) != GLFW_NO_ERROR)
        {
            std::cout << "could not create window context" << description << std::endl;
        }
        else
        {
            // set app icon
            GLFWimage icon;
            icon.pixels = stbi_load("resources/images/engine.png", &icon.width, &icon.height, 0, 4); //rgba channels 
            if (icon.pixels) 
            {
                glfwSetWindowIcon(m_Window, 1, &icon); 
                stbi_image_free(icon.pixels);
            }
            else
            {
                std::cout << "Could not load app icon " << std::endl;
            }
            
            // set scaling and aspect ratio 
            m_WindowScale = m_WindowWidth / 1280.0f;
            m_WindowAspectRatio = m_WindowHeight / (1.0f * m_WindowWidth);
            // all good
            ok = true;
        }
    }
    return ok;
}

bool Engine::WindowShouldClose() const
{ 
    return glfwWindowShouldClose(m_Window);
}

bool Engine::InitGLFW()
{
    
    // init glfw
    if (!glfwInit())
    {
        std::cout << "glfwInit() failed" << std::endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

bool Engine::InitGLEW()
{
    bool ok;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        ok =false;
        std::cout << "glewInit failed with error: " << glewGetErrorString(err) << std::endl;
    }
    else
    {
        ok = true;
        std::string infoMessage = "Using GLEW ";
        infoMessage += (char*)(glewGetString(GLEW_VERSION));
        Log::GetLogger()->info(infoMessage);
        
        if (GLEW_ARB_vertex_program)
        {
            Log::GetLogger()->info("ARB_vertex_program extension is supported");
        }
        
        if (GLEW_VERSION_1_3)
        {
            Log::GetLogger()->info("OpenGL 1.3 is supported");
        }
        
        if (glewIsSupported("GL_VERSION_1_4  GL_ARB_point_sprite"))
        {
            Log::GetLogger()->info("OpenGL 1.4 point sprites are supported");
        }
        
        if (glewGetExtension("GL_ARB_fragment_program"))
        {
            Log::GetLogger()->info("ARB_fragment_program is supported");
        }
        
        infoMessage = "Using OpenGL version ";
        infoMessage += (char*)glGetString(GL_VERSION);
        Log::GetLogger()->info(infoMessage);
    }
    
    std::cout << std::endl;
    return ok;
}

bool Engine::InitSDL()
{
    bool ok =false;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_JOYSTICK ) < 0 )
    {
        std::cout << "Could not initialize SDL. SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        ok = true;
    }
        
    return ok;
}
