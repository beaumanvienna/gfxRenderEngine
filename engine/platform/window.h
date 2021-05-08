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
#include <memory>

#include "engine.h"
#include "platform.h"
#include "event.h"

enum class WindowType
{
    OPENGL_WINDOW,
    VULKAN_WINDOW
};

struct WindowProperties
{
    std::string m_Title;
    int m_Width;
    int m_Height;
    bool m_VSync;
    
    WindowProperties(const std::string& title = "", const bool vsync = true,
                     const int width = -1, const int height = -1)
        : m_Title(title), m_VSync(vsync), m_Width(width), m_Height(height)
    {
    }
};

class Window
{
    
public:
    typedef std::function<void(Event&)> EventCallbackFunction;
    Window() {}
    virtual ~Window() {}
    
    virtual void* GetWindow() const = 0;
    virtual bool  IsOK() const = 0;
    virtual float GetWindowScale() const = 0;
    virtual float GetWindowAspectRatio() const = 0;
    virtual void  OnUpdate() = 0;
    virtual uint  GetWidth() const = 0;
    virtual uint  GetHeight() const = 0;
    
    virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    
    static std::unique_ptr<Window> Create(const WindowType windowType, const WindowProperties& props);

protected:
    
private:
    
};
