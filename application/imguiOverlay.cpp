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

#include "imguiOverlay.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "OpenGL/GL.h"
#include "imgui_engine.h"

void ImguiOverlay::OnAttach() 
{ 
    
    // projection matrix
    // orthographic matrix for projecting two-dimensional coordinates onto the screen

    // normalize to -0.5f - 0.5f
    normalizeX = 0.5f;
    normalizeY = 0.5f;

    // aspect ratio of image
    scaleTextureX = 1.0f;

    // aspect ratio of main window 
    m_ScaleMainWindowAspectRatio = m_Engine->GetWindowAspectRatio();

    // scale it to always have the same physical size on the screen
    // independently of the resolution
    scaleResolution = 1.0f / m_Engine->GetWindowScale();

    ortho_left   =-normalizeX * scaleResolution;
    ortho_right  = normalizeX * scaleResolution;
    ortho_bottom =-normalizeY * scaleResolution * m_ScaleMainWindowAspectRatio;
    ortho_top    = normalizeY * scaleResolution * m_ScaleMainWindowAspectRatio;
    ortho_near   =  1.0f;
    ortho_far    = -1.0f;

    normalizedPosition = glm::mat4
    (
        -0.5f,  0.5f, 1.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 1.0f, 1.0f
    );
}

void ImguiOverlay::OnDetach() 
{
    
}

void ImguiOverlay::OnEvent(Event& event) 
{
    event.MarkAsHandled();
}

void ImguiOverlay::OnUpdate() 
{
    ImguiUpdate((GLFWwindow*)m_Engine->GetWindow(), m_Engine->GetScaleImguiWidgets());
}
