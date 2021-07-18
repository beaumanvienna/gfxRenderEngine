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

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "application.h"
#include "input.h"
#include "renderCommand.h"
#include "controller.h"
#include "applicationEvent.h"
#include "controllerEvent.h"
#include "mouseEvent.h"
#include "keyEvent.h"

bool showGuybrush = true;
extern float zoomFactor;

std::unique_ptr<GameState> Application::m_GameState;

bool Application::Start()
{
    EngineApp::Start();
    
    m_GameState = std::make_unique<GameState>();
    m_GameState->Start();
    
    //enforce start-up aspect ratio when resizing the window
    Engine::m_Engine->SetWindowAspectRatio();
    
    m_SpritesheetMarley.AddSpritesheetPPSSPP("resources/atlas/atlas.png");

    m_Splash = new Splash(m_IndexBuffer, m_VertexBuffer, m_Renderer, "Splash Screen");
    Engine::m_Engine->PushLayer(m_Splash);
    
    m_SplashLogo = new SplashLogo(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, "splash logo");
    Engine::m_Engine->PushOverlay(m_SplashLogo);

    m_MainScreenBackground = new MainScreenBackground(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, "Main Screen Background");
    Engine::m_Engine->PushLayer(m_MainScreenBackground);

    m_Overlay = new Overlay(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, "Horn Overlay");
    Engine::m_Engine->PushOverlay(m_Overlay);
    
    m_UI = new UI(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, "UI");
    Engine::m_Engine->PushOverlay(m_UI);
    
    m_UIControllerIcon = new UIControllerIcon(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, "UI controller");
    Engine::m_Engine->PushOverlay(m_UIControllerIcon);
    
    m_TilemapLayer = new TilemapLayer(m_IndexBuffer, m_VertexBuffer, m_Renderer, "tilemap test");
    Engine::m_Engine->PushOverlay(m_TilemapLayer);

    m_ImguiOverlay = new ImguiOverlay(m_IndexBuffer, m_VertexBuffer, "Imgui Overlay");
    Engine::m_Engine->PushOverlay(m_ImguiOverlay);
    
    m_CameraController->SetTranslationSpeed(400.0f);
    m_CameraController->SetRotationSpeed(0.5f);
    
    m_EnableImgui = false;

    return true;
}

void Application::Shutdown()
{
    m_GameState->Shutdown();
    EngineApp::Shutdown();
}

void Application::OnUpdate()
{
    GameState::Scene scene = m_GameState->GetScene();
    
    m_CameraController->OnUpdate();

    //clear
    RenderCommand::Clear();

    // draw new scene
    m_Renderer->BeginScene(m_CameraController->GetCamera(), m_ShaderProg, m_VertexBuffer, m_IndexBuffer);

    switch(scene)
    {
        case GameState::SPLASH:
            m_Splash->OnUpdate();
            m_SplashLogo->OnUpdate();
            break;
        case GameState::MAIN:
            m_MainScreenBackground->OnUpdate();
            m_UI->OnUpdate();
            break;
    }

    m_TilemapLayer->OnUpdate();

    // --- endless loop Guybrush ---
    if (showGuybrush)
    {
        m_Overlay->OnUpdate();
    }
    if (!m_Splash->IsRunning())
    {
        m_UIControllerIcon->OnUpdate();
    }
    
    m_Renderer->Submit(m_VertexArray);
    m_Renderer->EndScene();
    
    m_GameState->OnUpdate();

    // update imgui widgets
    if (m_EnableImgui)
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
            return false;
        }
    );
    
    dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent event) 
        { 
            if (event.GetButton() == MouseButtonEvent::Left) 
            {
                // output context coordinates adjusted for orthographic projection
                float windowScale = Engine::m_Engine->GetWindowScale();
                float contextPositionX = event.GetX()/windowScale  - (Engine::m_Engine->GetContextWidth()/2.0f);
                float contextPositionY = (Engine::m_Engine->GetContextHeight()/2.0f) - event.GetY()/windowScale;
                LOG_APP_INFO("context position x: {0}, context position y: {1}", contextPositionX, contextPositionY);
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
    
    dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent event) 
        { 
            switch(event.GetKeyCode())
            {
                case ENGINE_KEY_I:
                    m_EnableImgui = !m_EnableImgui;
                    break;
                case ENGINE_KEY_R:
                    m_Overlay->SetRotation(0.0f);
                    m_CameraController->SetZoomFactor(1.0f);
                    m_CameraController->SetRotation(0.0f);
                    m_CameraController->SetTranslation({0.0f, 0.0f});
                    m_CameraController->SetProjection();
                    break;
            }
            return false;
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
