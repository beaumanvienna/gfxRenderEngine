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

bool showGuybrush = true;

bool Application::Start()
{
    EngineApp::Start();

    m_Splash = new Splash(&indexBuffer, &vertexBuffer, "Splash Screen");
    Engine::m_Engine->PushLayer(m_Splash);
    
    m_MainScreen = new MainScreenLayer(&indexBuffer, &vertexBuffer, "Main Screen");
    Engine::m_Engine->PushLayer(m_MainScreen);
    
    m_Overlay = new Overlay(&indexBuffer, &vertexBuffer, "Horn Overlay");
    Engine::m_Engine->PushOverlay(m_Overlay);
    
    m_ImguiOverlay = new ImguiOverlay(&indexBuffer, &vertexBuffer, "Imgui Overlay");
    Engine::m_Engine->PushOverlay(m_ImguiOverlay);

    return true;
}

void Application::Shutdown()
{
}

void Application::OnUpdate()
{
    if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_A))
        LOG_APP_INFO("Button A pressed");

    vertexBuffer.BeginDrawCall();
    indexBuffer.BeginDrawCall();

    if (m_Splash->IsRunning()) 
    {
        m_Splash->OnUpdate();
    } 
    else 
    {
        m_MainScreen->OnUpdate();
    }

    // --- endless loop horn ---
    if (showGuybrush)
    {
        m_Overlay->OnUpdate();
    }

    //clear
    renderer.Clear();

    // write index buffer
    indexBuffer.EndDrawCall();

    shaderProg.Bind();
    renderer.Draw(vertexArray,indexBuffer,shaderProg);

    // update imgui widgets
    if (!m_Splash->IsRunning())
    {
        m_ImguiOverlay->OnUpdate();
    }
}

void Application::OnEvent(Event& event)
{
}
