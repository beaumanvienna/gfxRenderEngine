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
        // create main window
        std::string title = "Engine v" ENGINE_VERSION;
        WindowProperties windowProperties(title, true);
        m_Window = Window::Create(WindowType::OPENGL_WINDOW, windowProperties);
        if (!m_Window->IsOK())
        {
            m_Running = false;
        }
        else
        {
            m_WindowWidth = m_Window->GetWidth();
            m_WindowHeight = m_Window->GetHeight();
            m_WindowScale = m_Window->GetWindowScale();
            m_WindowAspectRatio = m_Window->GetWindowAspectRatio();

            // init SDL
            if (!InitSDL())
            {
                m_Running = false;
            }
            else
            {
                // init imgui
                m_ScaleImguiWidgets = m_WindowScale * 1.4f; 
                if (!ImguiInit((GLFWwindow*)m_Window->GetWindow(), m_ScaleImguiWidgets))
                {
                    m_Running = false;
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

bool Engine::WindowShouldClose() const
{ 
    return glfwWindowShouldClose((GLFWwindow*)m_Window->GetWindow());
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
