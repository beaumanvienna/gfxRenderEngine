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
   
#include <csignal>
#include <filesystem>

#include "engine.h"
#include "core.h"
#include "input.h"
#include "log.h"
#include "imgui_engine.h"
#include "applicationEvent.h"
#include "controllerEvent.h"
#include "keyEvent.h"
#include "mouseEvent.h"
#include "application.h"
#include "renderCommand.h"

Engine*         Engine::m_Engine = nullptr;
SettingsManager Engine::m_SettingsManager;

Engine::Engine(int argc, char** argv, const std::string& configFilePath) :
            m_Argc(argc), m_Argv(argv), m_ConfigFilePath(configFilePath),
            m_Running(false), m_Paused(false), m_Window(nullptr), m_ScaleImguiWidgets(0),
            m_DisableMousePointerTimer(Timer(2500))
{
    #ifdef _MSC_VER
    m_HomeDir = "";
    #else
    m_HomeDir = getenv("HOME");
    #endif
    if (m_HomeDir == "")
    {
        auto path = std::filesystem::current_path();
        m_HomeDir = path.u8string();
    }
    
    if (m_HomeDir.substr(m_HomeDir.size() - 1) != "/")
    {
        m_HomeDir += "/";
    }

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

bool Engine::Start()
{
    m_Running = m_Paused = m_SwitchOffComputer = false;
    // init logger
    if (!Log::Init())
    {
        std::cout << "Could not initialize logger" << std::endl;
    }

    InitSettings();

    // set render API
    RendererAPI::SetAPI(m_CoreSettings.m_RendererAPI);
    
    // create main window
    std::string title = "Engine v" ENGINE_VERSION;
    WindowProperties windowProperties(title);
    m_Window = Window::Create(windowProperties);
    if (!m_Window->IsOK())
    {
        LOG_CORE_CRITICAL("Could not create main window");
        return false;
    }
    
    m_WindowScale = GetWindowWidth() / GetContextWidth();
    
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
    
    // init audio
    m_Audio = Audio::Create();
    m_Audio->Start();
    
    // init imgui
    m_ScaleImguiWidgets = GetWindowScale() * 2.0f;
    if (!ImguiInit((GLFWwindow*)m_Window->GetWindow(), m_ScaleImguiWidgets))
    {
        LOG_CORE_CRITICAL("Could not initialze imgui");
        return false;
    }
    m_TimeLastFrame = GetTime();   
    m_Running = true;
    
    signal(SIGINT, SignalHandler);

    return true;
}

void Engine::Shutdown(bool switchOffComputer)
{
    m_Running = false;
    m_SwitchOffComputer = m_SwitchOffComputer || switchOffComputer;
}

void Engine::Quit()
{
    m_LayerStack.Shutdown();

    // save settings
    m_CoreSettings.m_EngineVersion    = ENGINE_VERSION;
    m_CoreSettings.m_EnableFullscreen = IsFullscreen();
    m_SettingsManager.SaveToFile();

    if (m_SwitchOffComputer)
    {
        #ifdef LINUX
            std::string cmd = "shutdown now";
            FILE *fp;
            
            if ((fp = popen(cmd.c_str(), "r")) == nullptr) 
            {
                LOG_CORE_ERROR("Couldn't switch off computer: error opening pipe for command %s\n",cmd.c_str());
            }
        #endif
        
        #ifdef _WIN32
            system("C:\\WINDOWS\\System32\\shutdown /s /t 0");
        #endif
    }
}

void Engine::OnUpdate()
{
    //calculate time step
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
            RenderCommand::SetScissor(0, 0, event.GetWidth(), event.GetHeight());
            m_WindowScale = GetWindowWidth() / GetContextWidth();
            if ((event.GetWidth() == 0) || (event.GetHeight() == 0))
            {
                LOG_CORE_INFO("application paused");
                m_Paused = true;
            }
            else
            {
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

void Engine::SignalHandler(int signal)
{
    if (signal == SIGINT)
    {
        LOG_CORE_INFO("Received signal SIGINT, exiting");
        exit(0);
    }
}

void Engine::SetWindowAspectRatio()
{
    m_Window->SetWindowAspectRatio();
}

void Engine::ToggleFullscreen()
{
    m_Window->ToggleFullscreen();
}

void Engine::InitSettings()
{
    m_CoreSettings.InitDefaults();
    m_CoreSettings.RegisterSettings();

    // load external configuration 
    m_ConfigFilePath = GetHomeDirectory() + m_ConfigFilePath;
    std::string configFile = m_ConfigFilePath + "engine.cfg";
    
    m_SettingsManager.SetFilepath(configFile);
    m_SettingsManager.LoadFromFile();
    
    if (m_CoreSettings.m_EngineVersion != ENGINE_VERSION)
    {
        LOG_CORE_INFO("Welcome to engine version {0} (gfxRenderEngine)!", ENGINE_VERSION);
    }
    else
    {
        LOG_CORE_INFO("Starting engine (gfxRenderEngine) v" ENGINE_VERSION);
    }
}
void Engine::ApplyAppSettings()
{
    m_SettingsManager.ApplySettings();
}
