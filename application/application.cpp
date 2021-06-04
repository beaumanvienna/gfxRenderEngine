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

bool showGuybrush = true;
extern float debugTranslationX;
extern float debugTranslationY;

bool Application::Start()
{
    EngineApp::Start();
    
    m_SpritesheetMarley.AddSpritesheetPPSSPP("resources/images/ui_atlas/ui_atlas.png");

    m_Splash = new Splash(indexBuffer, vertexBuffer, m_Camera, "Splash Screen");
    Engine::m_Engine->PushLayer(m_Splash);

    m_MainScreen = new MainScreenLayer(indexBuffer, vertexBuffer, m_Camera, &m_SpritesheetMarley, "Main Screen");
    Engine::m_Engine->PushLayer(m_MainScreen);

    m_Overlay = new Overlay(indexBuffer, vertexBuffer, m_Camera, &m_SpritesheetMarley, "Horn Overlay");
    Engine::m_Engine->PushOverlay(m_Overlay);

    m_ImguiOverlay = new ImguiOverlay(indexBuffer, vertexBuffer, "Imgui Overlay");
    Engine::m_Engine->PushOverlay(m_ImguiOverlay);

    m_TranslationX = 0.0f;
    m_TranslationSpeed = 0.1f;

    m_Rotation = 0.0f;
    m_RotationSpeed = 0.5f;

    return true;
}

void Application::Shutdown()
{
}

void Application::OnUpdate()
{
    // camera control
    // move left and right based on controller input
    float cameraXMovement = 0.0f;
    if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_DPAD_LEFT)) 
    {
        cameraXMovement = +m_TranslationSpeed * Engine::m_Engine->GetTimestep();
    }
    else if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_DPAD_RIGHT)) 
    {
        cameraXMovement = -m_TranslationSpeed * Engine::m_Engine->GetTimestep();
    }
    if (cameraXMovement)
    {
        m_TranslationX += cameraXMovement;
        m_Camera->SetPosition( {m_TranslationX, 0.0f, 0.0f} );
    }
    // rotate based on controller input
    float cameraRotation = 0.0f;
    float leftTrigger  = Input::GetControllerTrigger(Controller::FIRST_CONTROLLER, Controller::LEFT_TRIGGER);
    float rightTrigger = Input::GetControllerTrigger(Controller::FIRST_CONTROLLER, Controller::RIGHT_TRIGGER);
    
    if (leftTrigger)
    {
        cameraRotation = -m_RotationSpeed * Engine::m_Engine->GetTimestep();
    }
    else if (rightTrigger) 
    {
        cameraRotation = +m_RotationSpeed * Engine::m_Engine->GetTimestep();
    }
    if (cameraRotation)
    {
        m_Rotation += cameraRotation;
        m_Camera->SetRotation( m_Rotation );
    }

    //clear
    RenderCommand::Clear();

    // draw new scene
    renderer->BeginScene();
    
    shaderProg->Bind();
    vertexBuffer->BeginScene();
    indexBuffer->BeginScene();

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
}
