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

#include "GLFWwindow.h"
#include "GLRenderer.h"
#include "core.h"
#include "log.h"
#include "stb_image.h"
#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"

bool GLFW_Window::m_GLFWIsInitialized = false;

GLFW_Window::GLFW_Window(const WindowProperties& props)
    : m_OK(false)
{
    m_WindowProperties.m_Title    = props.m_Title;
    m_WindowProperties.m_Width    = props.m_Width;
    m_WindowProperties.m_Height   = props.m_Height;
    m_WindowProperties.m_VSync    = props.m_VSync;

    m_OK = false;
    if (!m_GLFWIsInitialized)
    {
        // init glfw
        m_GLFWIsInitialized = InitGLFW();
        
    }
    if (m_GLFWIsInitialized)
    {
        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);
        m_RefreshRate = videoMode->refreshRate;

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
            LOG_CORE_CRITICAL("Failed to create main window");
            char description[1024];
            int errorCode = glfwGetError((const char **)(&description));

            if (errorCode != GLFW_NO_ERROR)
            {
                LOG_CORE_CRITICAL("glfw error code: {0}", errorCode);
            }
            glfwTerminate();
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
            
            m_GraphicsContext = GraphicsContext::Create(WindowType::OPENGL_WINDOW, m_Window, m_RefreshRate);
            if (!m_GraphicsContext->Init())
            {
                LOG_CORE_CRITICAL("Could not create a rendering context");
                
            }
            else
            {
                SetVSync(props.m_VSync);
            }

            // init glew
            if (InitGLEW())
            {
                // all good
                m_OK = true;
            }
        }
    }
}

GLFW_Window::~GLFW_Window()
{
    Shutdown();
}

void GLFW_Window::Shutdown()
{
    glfwDestroyWindow(m_Window);
}

void GLFW_Window::SetVSync(int interval)
{ 
    m_WindowProperties.m_VSync = interval;
    m_GraphicsContext->SetVSync(interval);
}

void GLFW_Window::OnUpdate()
{
    glfwPollEvents();
}

void GLFW_Window::OnError(int errorCode, const char* description) 
{
        LOG_CORE_CRITICAL("GLEW error, code: {0}, description: {1}", std::to_string(errorCode), description);

}

void GLFW_Window::SetEventCallback(const EventCallbackFunction& callback)
{
    m_WindowProperties.m_EventCallback = callback;
    glfwSetWindowUserPointer(m_Window,&m_WindowProperties);
    
    glfwSetErrorCallback([](int errorCode, const char* description) { GLFW_Window::OnError(errorCode, description);});
    
    glfwSetKeyCallback(m_Window,[](GLFWwindow* window, int key, int scancode, int action, int modes)
        {
            WindowData& windowProperties = *(WindowData*)glfwGetWindowUserPointer(window);
            EventCallbackFunction OnEvent = windowProperties.m_EventCallback;
            
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key);
                    OnEvent(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    OnEvent(event);
                    break;
                }
                case GLFW_REPEAT:
                    break;
            }
        }
    );
    
    glfwSetWindowCloseCallback(m_Window,[](GLFWwindow* window)
        {
            WindowData& windowProperties = *(WindowData*)glfwGetWindowUserPointer(window);
            EventCallbackFunction OnEvent = windowProperties.m_EventCallback;
                
            WindowCloseEvent event;
            OnEvent(event);
        }
    );
    
    glfwSetMouseButtonCallback(m_Window,[](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& windowProperties = *(WindowData*)glfwGetWindowUserPointer(window);
            EventCallbackFunction OnEvent = windowProperties.m_EventCallback;
            
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    OnEvent(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    OnEvent(event);
                    break;
                }
            }
        }
    );
    
    glfwSetCursorPosCallback(m_Window,[](GLFWwindow* window, double xpos, double ypos)
        {
            WindowData& windowProperties = *(WindowData*)glfwGetWindowUserPointer(window);
            EventCallbackFunction OnEvent = windowProperties.m_EventCallback;
                        
            MouseMovedEvent event(xpos, ypos);
            OnEvent(event);
            
        }
    );
    
    glfwSetScrollCallback(m_Window,[](GLFWwindow* window, double xoffset, double yoffset)
        {
            WindowData& windowProperties = *(WindowData*)glfwGetWindowUserPointer(window);
            EventCallbackFunction OnEvent = windowProperties.m_EventCallback;
                        
            MouseScrolledEvent event(xoffset, yoffset);
            OnEvent(event);
            
        }
    );
}

bool GLFW_Window::InitGLFW()
{
    
    // init glfw
    if (!glfwInit())
    {
        LOG_CORE_CRITICAL("glfwInit() failed");
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

bool GLFW_Window::InitGLEW()
{
    bool ok;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        ok =false;
        LOG_CORE_CRITICAL("glewInit failed with error: {0}", glewGetErrorString(err));
    }
    else
    {
        ok = true;
        LOG_CORE_INFO("Using GLEW {0}", (char*)(glewGetString(GLEW_VERSION)));
        
        if (GLEW_ARB_vertex_program)
        {
            LOG_CORE_INFO("ARB_vertex_program extension is supported");
        }
        
        if (GLEW_VERSION_1_3)
        {
            LOG_CORE_INFO("OpenGL 1.3 is supported");
        }
        
        if (glewIsSupported("GL_VERSION_1_4  GL_ARB_point_sprite"))
        {
            LOG_CORE_INFO("OpenGL 1.4 point sprites are supported");
        }
        
        if (glewGetExtension("GL_ARB_fragment_program"))
        {
            LOG_CORE_INFO("ARB_fragment_program is supported");
        }
        
        LOG_CORE_INFO("Using OpenGL version {0}", (char*)glGetString(GL_VERSION));
    }
    return ok;
}
