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
#include "input.h"
#include "log.h"
#include "imgui_engine.h"
#include "GLFW/GL.h"
#include "SDL.h"
#include "applicationEvent.h"
#include "controllerEvent.h"
#include "application.h"

// --- Class Engine ---
Engine* Engine::m_Engine = nullptr;
Engine::Engine(int argc, char** argv) :
            m_Running(false), m_Window(nullptr), m_WindowScale(0), m_ScaleImguiWidgets(0),
            m_WindowAspectRatio(0), m_WindowWidth(0), m_WindowHeight(0)
{
    m_Engine = this;
}

Engine::~Engine()
{
}

bool Engine::Start()
{
    m_Running = false;
    // init logger
    if (!Log::Init())
    {
        std::cout << "Could initialize logger" << std::endl;
    }
    else
    {
        // create main window
        std::string title = "Engine v" ENGINE_VERSION;
        WindowProperties windowProperties(title, true);
        m_Window = Window::Create(WindowType::OPENGL_WINDOW, windowProperties);
        if (!m_Window->IsOK())
        {
            LOG_CORE_CRITICAL("Could not create main window");
        }
        else
        {
            m_WindowWidth = m_Window->GetWidth();
            m_WindowHeight = m_Window->GetHeight();
            m_WindowScale = m_Window->GetWindowScale();
            m_WindowAspectRatio = m_Window->GetWindowAspectRatio();
            
            //setup callback
            m_Window->SetEventCallback([this](Event& event){ return this->OnEvent(event); });

            // init controller
            if (!m_Controller.Start())
            {
                LOG_CORE_CRITICAL("Could not create controller");
            }
            else
            {
                m_Controller.SetEventCallback([this](Event& event){ return this->OnEvent(event); });
                // init imgui
                m_ScaleImguiWidgets = m_WindowScale * 1.4f; 
                if (!ImguiInit((GLFWwindow*)m_Window->GetWindow(), m_ScaleImguiWidgets))
                {
                    LOG_CORE_CRITICAL("Could not initialze imgui");
                }
                else
                {
                    m_Running = true;
                }
            }
        }
    }

    if (!m_Running)
    {
        LOG_CORE_CRITICAL("Could not start engine, aborting");
    } 
    else
    {    
        LOG_CORE_INFO("Starting engine (gfxRenderEngine) v" ENGINE_VERSION);
    }    
    return m_Running;
}

void Engine::Shutdown()
{
    m_Running = false;
}

void Engine::OnUpdate()
{
    m_Window->OnUpdate();
    m_Controller.OnUpdate();
}

void Engine::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    
    // log events
    //if (event.GetCategoryFlags() & EventCategoryApplication) LOG_CORE_INFO(event);
    //if (event.GetCategoryFlags() & EventCategoryInput)       LOG_CORE_INFO(event);
    //if (event.GetCategoryFlags() & EventCategoryMouse)       LOG_CORE_INFO(event);
    //if (event.GetCategoryFlags() & EventCategoryController)  LOG_CORE_INFO(event);
    //if (event.GetCategoryFlags() & EventCategoryJoystick)    LOG_CORE_INFO(event);
    
    // dispatch to Engine
    dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent event) 
        { 
            Shutdown(); 
            return true;
        }
    );
    dispatcher.Dispatch<ControllerAxisMovedEvent>([this](ControllerAxisMovedEvent event) 
        { 
            Input::ControllerAxisMoved(event);
            return true;
        }
    );

    // also dispatch to application
    m_AppEventCallback(event);
    // and its layers
    for (auto layerIterator = m_LayerStack.end(); layerIterator != m_LayerStack.begin(); )
    {
        layerIterator--;
        (*layerIterator)->OnEvent(event);

        if (event.IsHandled()) break;
    }
}

void Engine::SetAppEventCallback(EventCallbackFunction eventCallback)
{
    m_AppEventCallback = eventCallback;
}
