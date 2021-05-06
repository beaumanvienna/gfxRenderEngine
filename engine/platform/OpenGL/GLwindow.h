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

#pragma once

#include <iostream>
#include <functional>

#include "engine.h"
#include "platform.h"
#include "OpenGL/GL.h"
#include "window.h"
#include "event.h"

class GLWindow : public Window
{
    
public:
    
    GLWindow(const WindowProperties& props);
    ~GLWindow() override;
    
    void OnUpdate() override;
    uint GetWidth()  const override { return m_WindowProperties.m_Width; }
    uint GetHeight() const override { return m_WindowProperties.m_Height; }
    
    void SetEventCallback(const EventCallbackFunction& callback) override;
    void SetVSync(bool enabled) override { m_VSync = enabled; }
    bool IsOK() const { return m_OK; }
    float GetWindowScale() const { return m_WindowScale; }
    float GetWindowAspectRatio() const { return m_WindowAspectRatio; }
    GLFWwindowPtr GetWindow() const { return m_Window; }
    
    static Window* Create(const WindowProperties& props = WindowProperties());

protected:
    
private:

    bool m_OK;

    WindowProperties m_WindowProperties;
    GLFWwindowPtr m_Window;
    bool m_VSync = false;
    
    float m_WindowScale, m_WindowAspectRatio;
    
};
