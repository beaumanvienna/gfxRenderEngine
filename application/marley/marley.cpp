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
#include "renderCommand.h"

#include "input.h"
#include "marley.h"
#include "controller.h"
#include "renderCommand.h"
#include "applicationEvent.h"
#include "marley/emulation/emulationEvent.h"
#include "marley/characters/characterEvent.h"
#include "orthographicCameraController.h"
#include "controllerEvent.h"
#include "mouseEvent.h"
#include "resources.h"
#include "keyEvent.h"

namespace MarleyApp
{

    std::unique_ptr<GameState> Marley::m_GameState;
    std::unique_ptr<AutoMove> Marley::m_AutoMoveCharacter;
    std::unique_ptr<EmulationUtils> Marley::m_EmulationUtils;
    Marley* Marley::m_Application;
    Bios Marley::m_BiosFiles;

    std::string Marley::GetConfigFilePath()
    {
        return ".marley/";
    }

    bool Marley::Start()
    {

        EngineApp::Start();
        InitSettings();
        InitCursor();

        m_Application = this;

        m_GameState = std::make_unique<GameState>();
        m_GameState->SetEventCallback([](AppEvent& event){ return Marley::m_Application->OnAppEvent(event);});
        m_GameState->Start();

        m_EmulationUtils = std::make_unique<EmulationUtils>();
        m_EmulationUtils->CreateConfigFolder();
        m_BiosFiles.SetBaseDirectory();

        m_AutoMoveCharacter = std::make_unique<AutoMove>();

        //enforce start-up aspect ratio when resizing the window
        Engine::m_Engine->SetWindowAspectRatio();

        m_SpritesheetMarley.AddSpritesheet("/images/atlas/atlas.png", IDB_ATLAS, "PNG");

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

        m_MessageBoard = new MessageBoard(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, "message board");
        Engine::m_Engine->PushOverlay(m_MessageBoard);

        m_UIStarIcon = new UIStarIcon(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, true, "UI star icon splash");
        Engine::m_Engine->PushOverlay(m_UIStarIcon);

        m_EmulatorLayer = new EmulatorLayer(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, m_Overlay, "EmulatorLayer");
        Engine::m_Engine->PushOverlay(m_EmulatorLayer);

        m_Instructions = new Instructions(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, "InstructionsLayer");
        Engine::m_Engine->PushOverlay(m_Instructions);

        m_TilemapLayer = new TilemapLayer(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, "tilemap test");
        Engine::m_Engine->PushOverlay(m_TilemapLayer);

        m_ImguiOverlay = new ImguiOverlay(m_IndexBuffer, m_VertexBuffer, "Imgui Overlay");
        Engine::m_Engine->PushOverlay(m_ImguiOverlay);

        m_EmulatorLayer->SetInstructions(m_Instructions);
        m_CameraController->SetTranslationSpeed(400.0f);
        m_CameraController->SetRotationSpeed(0.5f);

        m_EnableImgui = false;

        return true;
    }

    void Marley::Shutdown()
    {
        m_GameState->Shutdown();
        EngineApp::Shutdown();
    }

    void Marley::OnUpdate()
    {
        m_CameraController->OnUpdate();

        //clear
        RenderCommand::Clear();

        // draw new scene
        m_Renderer->BeginScene(m_CameraController->GetCamera(), m_ShaderProg, m_VertexBuffer, m_IndexBuffer);

        GameState::Scene scene = m_GameState->GetScene();
        m_GameState->OnUpdate();
        GameState::EmulationMode emulationMode = m_GameState->GetEmulationMode();

        // splash and background
        switch(scene)
        {
            case GameState::SPLASH:
                m_Splash->OnUpdate();
                m_SplashLogo->OnUpdate();

                // message board
                if (!m_Splash->IsRunning())
                {
                    if (!m_UIControllerIcon->IsMovingIn())
                    {
                        m_MessageBoard->Start();
                    }
                }
                break;
            case GameState::MAIN:
                m_MainScreenBackground->OnUpdate();
                m_MessageBoard->Stop();
                break;
        }

        // message board stars
        if (m_MessageBoard->IsRunning())
        {
            m_UIStarIcon->Start();
        }
        else
        {
            m_UIStarIcon->Stop();
        }

        // show controller icon
        if (!m_Splash->IsRunning())
        {
            m_UIControllerIcon->OnUpdate();
        }

        //emulator
        if (emulationMode != GameState::OFF)
        {
            m_EmulatorLayer->BeginScene();
            m_EmulatorLayer->OnUpdate();
            m_EmulatorLayer->EndScene();
            m_Instructions->Start();
            m_Instructions->OnUpdate();
        }

        //GUI
        switch(scene)
        {
            case GameState::MAIN:
                m_UI->OnUpdate();
                break;
        }

        m_MessageBoard->OnUpdate();
        m_UIStarIcon->OnUpdate();

        m_TilemapLayer->OnUpdate();



        // --- endless loop Guybrush ---
        if (showGuybrush)
        {
            m_Overlay->OnUpdate();
        }

        m_Renderer->Submit(m_VertexArray);
        m_Renderer->EndScene();

        // update imgui widgets
        if (m_EnableImgui)
        {
            m_ImguiOverlay->OnUpdate();
            m_Cursor->RestoreCursor();
        }

    }

    void Marley::OnEvent(Event& event)
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
                if ((event.GetControllerButton() == Controller::BUTTON_GUIDE) && (m_GameState->GetScene()==GameState::SPLASH))
                {
                    Shutdown();
                }
                return false;
            }
        );

        dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent event)
            {
                if (event.GetButton() == MouseButtonEvent::Right)
                {
                    // output context coordinates adjusted for orthographic projection
                    float windowScale = Engine::m_Engine->GetWindowScale();
                    float contextPositionX = event.GetX()/windowScale  - (Engine::m_Engine->GetContextWidth()/2.0f);
                    float contextPositionY = (Engine::m_Engine->GetContextHeight()/2.0f) - event.GetY()/windowScale;
                    LOG_APP_INFO("context position x: {0}, context position y: {1}", contextPositionX, contextPositionY);
                    m_AutoMoveCharacter->SetDestination(contextPositionX, contextPositionY);
                }

                return false;
            }
        );

        dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent event)
            {
                if (Input::IsKeyPressed(KeyCode::ENGINE_KEY_LEFT_CONTROL))
                {
                    zoomFactor -= event.GetY()*0.1f;
                    OnScroll();
                    return true;
                }
                return false;
            }
        );

        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent event)
            {
                switch(event.GetKeyCode())
                {
                    case ENGINE_KEY_M:
                        m_EnableImgui = !m_EnableImgui;
                        m_CameraController->SetEnable(true);
                        break;
                    case ENGINE_KEY_ESCAPE:
                        if (m_GameState->GetScene()==GameState::SPLASH) Shutdown();
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

    void Marley::OnAppEvent(AppEvent& event)
    {
        AppEventDispatcher appDispatcher(event);

        appDispatcher.Dispatch<EmulatorLaunchEvent>([this](EmulatorLaunchEvent event)
            {
                m_EmulatorLayer->SetGameFilename(event.GetGameFilename());
                m_GameState->SetEmulationMode(GameState::RUNNING);
                UI::m_ScreenManager->RecreateAllViews();
                return true;
            }
        );

        appDispatcher.Dispatch<InputIdleEvent>([this](InputIdleEvent event)
            {
                m_AutoMoveCharacter->SetActivationState(event.IsIdle());
                return true;
            }
        );

        m_EmulatorLayer->OnAppEvent(event);
    }

    void Marley::OnResize()
    {
        m_CameraController->SetProjection();
    }

    void Marley::OnScroll()
    {
        m_CameraController->SetProjection();
    }

    void Marley::InitSettings()
    {
        m_AppSettings.InitDefaults();
        m_AppSettings.RegisterSettings();

        // apply external settings
        Engine::m_Engine->ApplyAppSettings();
    }

    void Marley::InitCursor()
    {
        m_Cursor = Cursor::Create();

        size_t fileSize;
        const uchar* data = (const uchar*) ResourceSystem::GetDataPointer(fileSize, "/images/images/cursor.png", IDB_CURSOR_RETRO, "PNG");
        m_Cursor->SetCursor(data, fileSize, 32, 32);
        Engine::m_Engine->AllowCursor();
    }

    void Marley::Flush()
    {
        m_Renderer->Submit(m_VertexArray);
        m_Renderer->EndScene();
        m_Renderer->BeginScene(m_CameraController->GetCamera(), m_ShaderProg, m_VertexBuffer, m_IndexBuffer);
    }

    std::chrono::time_point<std::chrono::steady_clock> Marley::GetSplashStartTime() const
    {
        return m_Splash->GetStartTime();
    }
}
