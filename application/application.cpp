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

bool showGuybrush = true;
extern float debugTranslationX;
extern float debugTranslationY;

bool Application::Start()
{
    EngineApp::Start();
    
    //enforce start-up aspect ratio when resizing the window
    Engine::m_Engine->SetWindowAspectRatio();
    
    m_SpritesheetMarley.AddSpritesheetPPSSPP("resources/images/ui_atlas/ui_atlas.png");

    m_Splash = new Splash(indexBuffer, vertexBuffer, m_Camera, "Splash Screen");
    Engine::m_Engine->PushLayer(m_Splash);

    m_MainScreen = new MainScreenLayer(indexBuffer, vertexBuffer, m_Camera, &m_SpritesheetMarley, "Main Screen");
    Engine::m_Engine->PushLayer(m_MainScreen);

    m_Overlay = new Overlay(indexBuffer, vertexBuffer, m_Camera, &m_SpritesheetMarley, "Horn Overlay");
    Engine::m_Engine->PushOverlay(m_Overlay);

    m_ImguiOverlay = new ImguiOverlay(indexBuffer, vertexBuffer, "Imgui Overlay");
    Engine::m_Engine->PushOverlay(m_ImguiOverlay);
    
    m_CameraController->SetTranslationSpeed(0.1f);
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
    renderer->BeginScene(m_CameraController->GetCamera(), shaderProg, vertexBuffer, indexBuffer);

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
    
    renderer->Submit(vertexArray);
    renderer->EndScene();

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
}

void Application::OnResize()
{
    m_CameraController->SetProjection();
}
