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
#include "GL.h"
#include "window.h"
#include "event.h"
#include "graphicsContext.h"

class GLFW_Window : public Window
{
    
public:
    
    GLFW_Window(const WindowProperties& props);
    ~GLFW_Window() override;
    
    bool InitGLEW();
    bool InitGLFW();
    void Shutdown();
    void* GetWindow() const override { return (void*)m_Window; }
    std::shared_ptr<GraphicsContext> GetGraphicsContent() const override { return m_GraphicsContext; }
    void OnUpdate() override;    
    uint GetWidth()  const override { return m_WindowProperties.m_Width; }
    uint GetHeight() const override { return m_WindowProperties.m_Height; }
    
    void SetEventCallback(const EventCallbackFunction& callback) override;
    void SetVSync(int interval) override;
    void ToggleFullscreen() override;
    bool IsFullscreen() override { return m_IsFullscreen; }
    bool IsOK() const override { return m_OK; }
    void SetWindowAspectRatio() override;
    void SetWindowAspectRatio(int numer, int denom) override;
    float GetWindowAspectRatio() const override { return m_WindowProperties.m_Width / (1.0f * m_WindowProperties.m_Height); }
    double GetTime() const override { return glfwGetTime(); }
    
    static void OnError(int errorCode, const char* description);
    
    void EnableMousePointer() override;
    void DisableMousePointer() override;

protected:

private:

    struct WindowData
    {
        std::string m_Title;
        int m_Width;
        int m_Height;
        int m_VSync;
        EventCallbackFunction m_EventCallback;
        double m_MousePosX;
        double m_MousePosY;
    };

    static bool m_GLFWIsInitialized;

    bool m_OK;

    WindowData m_WindowProperties;
    GLFWwindow* m_Window;
    std::shared_ptr<GraphicsContext>(m_GraphicsContext);

    uint m_RefreshRate;
    bool m_IsFullscreen;
    
    int m_WindowedWidth, m_WindowedHeight;
    int m_WindowPositionX, m_WindowPositionY;
};
