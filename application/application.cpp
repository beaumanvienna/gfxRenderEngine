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

extern bool showGuybrush;

bool Application::Init(Engine* engine)
{
    EngineApp::Init(engine);

    m_Splash = new Splash(m_Engine, &indexBuffer, &vertexBuffer, "Splash Screen");
    m_Engine->PushLayer(m_Splash);
    
    m_MainScreen = new MainScreenLayer(m_Engine, &indexBuffer, &vertexBuffer, "Main Screen");
    m_Engine->PushLayer(m_MainScreen);
    
    m_Overlay = new Overlay(m_Engine, &indexBuffer, &vertexBuffer, "Horn Overlay");
    m_Engine->PushOverlay(m_Overlay);
    
    m_ImguiOverlay = new ImguiOverlay(m_Engine, &indexBuffer, &vertexBuffer, "Imgui Overlay");
    m_Engine->PushOverlay(m_ImguiOverlay);

    return true;
}

void Application::OnEvent(Event& event)
{
}

void Application::OnUpdate()
{

    vertexBuffer.BeginDrawCall();
    indexBuffer.BeginDrawCall();

    if (m_Splash->IsRunning()) 
    {
        m_Splash->OnUpdate();
        
    } else 
    {
        if (m_MainScreen)
        {
            m_MainScreen->OnUpdate();
        }
        else
        {
            LOG_APP_CRITICAL("main screen not set");
        }
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

    renderer.SwapBuffers();
}
