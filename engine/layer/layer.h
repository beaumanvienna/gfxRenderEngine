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
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
    
   The code in this file is based on and inspired by the project
   https://github.com/TheCherno/Hazel. The license of this prject can
   be found under https://github.com/TheCherno/Hazel/blob/master/LICENSE
   */

#pragma once

#include "engine.h"
#include "platform.h"

class Event;

class Layer
{
public:

    Layer(const std::string& name = "layer");
    virtual ~Layer();
    
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnEvent(Event& event) {}
    virtual void OnUpdate() {}

    inline const std::string& GetName() const { return m_DebugName; }

private: 
    bool m_Enabled;
    std::string m_DebugName;
};


#define INIT_LAYER(empty)                                                                   \
                                                                                            \
    /* projection matrix */                                                                 \
    /* orthographic matrix for projecting two-dimensional coordinates onto the screen */    \
                                                                                            \
    /* normalize to -0.5f - 0.5f */                                                         \
    normalizeX = 0.5f;                                                                      \
    normalizeY = 0.5f;                                                                      \
                                                                                            \
    /* aspect ratio of image */                                                             \
    scaleTextureX = 1.0f;                                                                   \
                                                                                            \
    /* aspect ratio of main window */                                                       \
    m_ScaleMainWindowAspectRatio = Engine::m_Engine->GetWindowAspectRatio();                \
                                                                                            \
    /* scale it to always have the same physical size on the screen */                      \
    /* independently of the resolution */                                                   \
    scaleResolution = 1.0f / Engine::m_Engine->GetWindowScale();                            \
                                                                                            \
    ortho_left   =-normalizeX * scaleResolution;                                            \
    ortho_right  = normalizeX * scaleResolution;                                            \
    ortho_bottom =-normalizeY * scaleResolution * m_ScaleMainWindowAspectRatio;             \
    ortho_top    = normalizeY * scaleResolution * m_ScaleMainWindowAspectRatio;             \
    ortho_near   =  1.0f;                                                                   \
    ortho_far    = -1.0f;                                                                   \
                                                                                            \
    normalizedPosition = glm::mat4                                                          \
    (                                                                                       \
        -0.5f,  0.5f, 1.0f, 1.0f,                                                           \
         0.5f,  0.5f, 1.0f, 1.0f,                                                           \
         0.5f, -0.5f, 1.0f, 1.0f,                                                           \
        -0.5f, -0.5f, 1.0f, 1.0f                                                            \
    );
