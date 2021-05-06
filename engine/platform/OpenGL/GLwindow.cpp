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

#include "GLwindow.h"
#include "log.h"
#include "stb_image.h"

GLWindow::GLWindow(const WindowProperties& props)
    : m_VSync(false), m_OK(false)
{
    m_WindowProperties.m_Title  = props.m_Title;
    m_WindowProperties.m_Width  = props.m_Width;
    m_WindowProperties.m_Height = props.m_Height;
    
    m_OK = false;
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);
    
    if (m_WindowProperties.m_Width == -1)
    {
        m_WindowProperties.m_Width  = videoMode->width / 1.5f;
        m_WindowProperties.m_Height = m_WindowProperties.m_Width / 16 * 9;
    }
    int monitorX, monitorY;
    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

    // make window invisible before it gets centered
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    
    m_Window = glfwCreateWindow(m_WindowProperties.m_Width, m_WindowProperties.m_Height, m_WindowProperties.m_Title.c_str(), NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        std::cout << "Failed to create main window" << std::endl;
    }
    else
    {
        // center window
        glfwSetWindowPos(m_Window,
                         monitorX + (videoMode->width - m_WindowProperties.m_Width) / 2,
                         monitorY + (videoMode->height - m_WindowProperties.m_Height) / 2);
        
        // make the centered window visible
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwShowWindow(m_Window);
        
        // create context
        char description[1024];
        glfwMakeContextCurrent(m_Window);
        if (glfwGetError((const char**)(&description)) != GLFW_NO_ERROR)
        {
            std::cout << "could not create window context" << description << std::endl;
        }
        else
        {
            // set app icon
            GLFWimage icon;
            icon.pixels = stbi_load("resources/images/engine.png", &icon.width, &icon.height, 0, 4); //rgba channels 
            if (icon.pixels) 
            {
                glfwSetWindowIcon(m_Window, 1, &icon); 
                stbi_image_free(icon.pixels);
            }
            else
            {
                std::cout << "Could not load app icon " << std::endl;
            }
            
            // set scaling and aspect ratio 
            m_WindowScale = m_WindowProperties.m_Width / 1280.0f;
            m_WindowAspectRatio = m_WindowProperties.m_Height / (1.0f * m_WindowProperties.m_Width);
            // all good
            m_OK = true;
        }
    }
    
}

GLWindow::~GLWindow()
{
}

void GLWindow::OnUpdate()
{
}

void GLWindow::SetEventCallback(const EventCallbackFunction& callback)
{
}

Window* GLWindow::Create(const WindowProperties& props)
{
    Window* window = new GLWindow(props);
    return window;
}
