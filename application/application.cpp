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

#include <memory>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "application.h"
#include "input.h"
#include "renderCommand.h"
#include "controller.h"
#include "applicationEvent.h"
#include "controllerEvent.h"
#include "mouseEvent.h"

bool showGuybrush = true;
extern float zoomFactor;

bool Application::Start()
{
    EngineApp::Start();
    
    //enforce start-up aspect ratio when resizing the window
    Engine::m_Engine->SetWindowAspectRatio();
    
    m_SpritesheetMarley.AddSpritesheetPPSSPP("resources/images/ui_atlas/ui_atlas.png");

    m_Splash = new Splash(m_IndexBuffer, m_VertexBuffer, m_Camera, m_Renderer, "Splash Screen");
    Engine::m_Engine->PushLayer(m_Splash);

    m_MainScreen = new MainScreenLayer(m_IndexBuffer, m_VertexBuffer, m_Camera, m_Renderer, &m_SpritesheetMarley, "Main Screen");
    Engine::m_Engine->PushLayer(m_MainScreen);

    m_Overlay = new Overlay(m_IndexBuffer, m_VertexBuffer, m_Camera, m_Renderer, &m_SpritesheetMarley, "Horn Overlay");
    Engine::m_Engine->PushOverlay(m_Overlay);

    m_ImguiOverlay = new ImguiOverlay(m_IndexBuffer, m_VertexBuffer, "Imgui Overlay");
    Engine::m_Engine->PushOverlay(m_ImguiOverlay);
    
    m_CameraController->SetTranslationSpeed(400.0f);
    m_CameraController->SetRotationSpeed(0.5f);

    return true;
}

void Application::Shutdown()
{
    EngineApp::Shutdown();
}

void Application::OnUpdate()
{
    m_CameraController->OnUpdate();

    //clear
    RenderCommand::Clear();

    // draw new scene
    m_Renderer->BeginScene(m_CameraController->GetCamera(), m_ShaderProg, m_VertexBuffer, m_IndexBuffer);

    if (m_Splash->IsRunning()) 
    {
        m_Splash->OnUpdate();
    } 
    else 
    {
        m_MainScreen->OnUpdate();
    }

    // --- endless loop Guybrush ---
    if (showGuybrush)
    {
        m_Overlay->OnUpdate();
    }
    
    m_Renderer->Submit(m_VertexArray);
    m_Renderer->EndScene();

    // update imgui widgets
    if (!m_Splash->IsRunning())
    {
        m_ImguiOverlay->OnUpdate();
    }
}

void Application::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    
    dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent event) 
        { 
            OnResize();
            return true;
        }
    );
    
    dispatcher.Dispatch<ControllerButtonPressedEvent>([this](ControllerButtonPressedEvent event) 
        { 
            if (event.GetControllerButton() == Controller::BUTTON_GUIDE) Shutdown();
            return true;
        }
    );
    
    dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent event) 
        { 
            if (event.GetButton() == MouseButtonEvent::Left) 
            {
                float windowScale = Engine::m_Engine->GetWindowScale();
                LOG_APP_INFO("window position x: {0}, window position y: {1}, context position x: {2}, context position y: {3}", event.GetX(), event.GetY(), event.GetX()/windowScale, event.GetY()/windowScale);
            }
            return true;
        }
    );
    
    dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent event) 
        { 
            zoomFactor -= event.GetY()*0.1f;
            OnScroll();
            return true;
        }
    );
}

void Application::OnResize()
{
    m_CameraController->SetProjection();
}

void Application::OnScroll()
{
    m_CameraController->SetProjection();
}
