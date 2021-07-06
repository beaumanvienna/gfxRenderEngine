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
#include "applicationEvent.h"
#include "controllerEvent.h"
#include "keyEvent.h"
#include "mouseEvent.h"
#include "application.h"

// --- Class Engine ---
Engine* Engine::m_Engine = nullptr;
Engine::Engine(int argc, char** argv) :
            m_Running(false), m_Paused(false), m_Window(nullptr), m_ScaleImguiWidgets(0),
            m_DisableMousePointerTimer(Timer(2500))
{
    m_Engine = this;
    
    m_DisableMousePointerTimer.SetEventCallback([](uint interval, void* parameters)
        {
            uint returnValue = 0;
            int timerID = *((int*)parameters);
            Engine::m_Engine->DisableMousePointer();
            return returnValue;
        }
    );
}

Engine::~Engine()
{
}

bool Engine::Start(RendererAPI::API api)
{
    m_Running = m_Fullscreen = false;
    // init logger
    if (!Log::Init())
    {
        std::cout << "Could not initialize logger" << std::endl;
    }
    else
    {
        LOG_CORE_INFO("Starting engine (gfxRenderEngine) v" ENGINE_VERSION);
    }
    
    // set render API
    RendererAPI::SetAPI(api);
    
    // create main window
    std::string title = "Engine v" ENGINE_VERSION;
    WindowProperties windowProperties(title);
    m_Window = Window::Create(windowProperties);
    if (!m_Window->IsOK())
    {
        LOG_CORE_CRITICAL("Could not create main window");
        return false;
    }
    
    //setup callback
    m_Window->SetEventCallback([this](Event& event){ return this->OnEvent(event); });
    m_GraphicsContext = m_Window->GetGraphicsContent();
    
    // init controller
    if (!m_Controller.Start())
    {
        LOG_CORE_CRITICAL("Could not create controller");
        return false;
    }
    else
    {
        m_Controller.SetEventCallback([this](Event& event){ return this->OnEvent(event); });
    }
    
    // init imgui
    m_ScaleImguiWidgets = m_Window->GetWindowScale() * 2.0f;
    if (!ImguiInit((GLFWwindow*)m_Window->GetWindow(), m_ScaleImguiWidgets))
    {
        LOG_CORE_CRITICAL("Could not initialze imgui");
        return false;
    }
    m_TimeLastFrame = GetTime();   
    m_Running = true;

    return true;
}

void Engine::Shutdown()
{
    m_Running = false;
}

void Engine::OnUpdate()
{
    //claculate time step
    float time = (float)GetTime();
    m_Timestep =  time - m_TimeLastFrame;
    m_TimeLastFrame = time;

    m_Window->OnUpdate();
    m_Controller.OnUpdate();
}

void Engine::OnRender()
{
    m_GraphicsContext->SwapBuffers();
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
    
    dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent event) 
        { 
            if ((event.GetWidth() == 0) || (event.GetHeight() == 0))
            {
                LOG_CORE_INFO("application paused");
                m_Paused = true;
            }
            else
            {
                LOG_CORE_INFO("application running");
                m_Paused = false;
            }
            return true;
        }
    );
    
    dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent event) 
        { 
            switch(event.GetKeyCode())
            {
                case ENGINE_KEY_F:
                    ToggleFullscreen();
                    break;
                case ENGINE_KEY_ESCAPE:
                    Shutdown();
                    break;
            }
            return false;
        }
    );
    
    dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent event) 
        { 
            m_Window->EnableMousePointer();
            m_DisableMousePointerTimer.Stop();
            m_DisableMousePointerTimer.Start();
            return true;
        }
    );

    // also dispatch to application
    if (!event.IsHandled()) m_AppEventCallback(event);
    // and its layers
    if (!event.IsHandled())
    {
        for (auto layerIterator = m_LayerStack.end(); layerIterator != m_LayerStack.begin(); )
        {
            layerIterator--;
            (*layerIterator)->OnEvent(event);
    
            if (event.IsHandled()) break;
        }
    }
}

void Engine::SetAppEventCallback(EventCallbackFunction eventCallback)
{
    m_AppEventCallback = eventCallback;
}

void Engine::SetWindowAspectRatio()
{
    m_Window->SetWindowAspectRatio();
}

void Engine::ToggleFullscreen()
{
    m_Fullscreen = !m_Fullscreen;
    m_Window->ToggleFullscreen();
}
