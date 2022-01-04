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

#include "scabb.h"
#include "input.h"
#include "renderCommand.h"
#include "controller.h"
#include "applicationEvent.h"
#include "controllerEvent.h"
#include "mouseEvent.h"
#include "keyEvent.h"
#include "resources.h"
#include "orthographicCameraController.h"

namespace ScabbApp
{
    Scabb* Scabb::m_Application;

    std::string Scabb::GetConfigFilePath()
    {
        std::string configFilePath{".scabb"};
        return EngineCore::AddSlash(configFilePath);
    }

    bool Scabb::Start()
    {

        EngineApp::Start();
        InitSettings();
        InitCursor();

        m_Application = this;

        //enforce start-up aspect ratio when resizing the window
        Engine::m_Engine->SetWindowAspectRatio();

        m_SpritesheetMarley.AddSpritesheet("/images/atlas/atlas.png", IDB_ATLAS, "PNG");

        // create layers
        m_Background = new Background(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley, "Main Screen Background");
        Engine::m_Engine->PushLayer(m_Background);

        m_RayTracing = new RayTracing(m_IndexBuffer, m_VertexBuffer, m_Renderer, &m_SpritesheetMarley);
        Engine::m_Engine->PushLayer(m_RayTracing);

        m_CameraController->SetTranslationSpeed(400.0f);
        m_CameraController->SetRotationSpeed(0.5f);

        return true;
    }

    void Scabb::Shutdown()
    {
        EngineApp::Shutdown();
    }

    void Scabb::OnUpdate()
    {

        m_CameraController->OnUpdate();

        //clear
        RenderCommand::Clear();

        // draw new scene
        m_Renderer->BeginScene(m_CameraController->GetCamera(), m_ShaderProg, m_VertexBuffer, m_IndexBuffer);

        // OnUpdate layers
        m_Background->OnUpdate();
        m_RayTracing->OnUpdate();

        m_Renderer->Submit(m_VertexArray);
        m_Renderer->EndScene();

    }

    void Scabb::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent event)
            {
                OnResize();
                return true;
            }
        );

        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent event)
            {
                switch(event.GetKeyCode())
                {
                    case ENGINE_KEY_ESCAPE:
                        Shutdown();
                        break;
                    case ENGINE_KEY_R:
                        m_CameraController->SetZoomFactor(1.0f);
                        m_CameraController->SetRotation(0.0f);
                        m_CameraController->SetTranslation({0.0f, 0.0f});
                        m_CameraController->SetProjection();
                        break;
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
    }

    void Scabb::OnResize()
    {
        m_CameraController->SetProjection();
    }

    void Scabb::OnScroll()
    {
        m_CameraController->SetProjection();
    }

    void Scabb::InitSettings()
    {
        CreateConfigFolder();
        //m_AppSettings.InitDefaults();
        //m_AppSettings.RegisterSettings();
        //
        //// apply external settings
        //Engine::m_Engine->ApplyAppSettings();
    }

    void Scabb::InitCursor()
    {
        m_Cursor = Cursor::Create();

        size_t fileSize;
        const uchar* data = (const uchar*) ResourceSystem::GetDataPointer(fileSize, "/images/images/cursor.png", IDB_CURSOR_RETRO, "PNG");
        m_Cursor->SetCursor(data, fileSize, 32, 32);
    }

    void Scabb::Flush()
    {
        m_Renderer->Submit(m_VertexArray);
        m_Renderer->EndScene();
        m_Renderer->BeginScene(m_CameraController->GetCamera(), m_ShaderProg, m_VertexBuffer, m_IndexBuffer);
    }

    void Scabb::CreateConfigFolder()
    {
        std::string configFilePath = Engine::m_Engine->GetConfigFilePath();
        if (!EngineCore::FileExists(configFilePath))
        {
            if (EngineCore::CreateDirectory(configFilePath))
            {
                LOG_APP_INFO("Configuration folder {0} created", configFilePath);
            }
            else
            {
                LOG_APP_CRITICAL("Couldn't create configuration folder {0}", configFilePath);
            }
        }
    }
}
