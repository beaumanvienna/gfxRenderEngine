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

#include "GLGraphicsContext.h"


GLContext::GLContext(GLFWwindow* window)
    : m_Window(window)
{
}

bool GLContext::Init()
{
    m_Initialized = false;
    char description[1024];
    
    //clear error
    GLFWClearError();
    
    // create context
    glfwMakeContextCurrent(m_Window);
    
    // get error
    int errorCode = glfwGetError((const char**)(&description));
    
    // test error code
    if ( errorCode != GLFW_NO_ERROR)
    {
        LOG_CORE_CRITICAL("could not create window context, error code {0}", errorCode);
    }
    else
    {
        m_Initialized = true;
    }
    
    return m_Initialized;
}

void GLContext::SwapBuffers()
{
    usleep(32000); // ~30 frames per second (in micro (!) seconds)
    GLCall(glfwSwapBuffers(m_Window));
}
