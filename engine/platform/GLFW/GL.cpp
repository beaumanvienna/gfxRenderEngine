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

#include "GLFW/GL.h"

static const uint MAX_CLEAR_ERROR_CALLS = 10; // give up clearing faults after a specified number of calls

void GLClearError()
{
    uint timeout = MAX_CLEAR_ERROR_CALLS;
    while (glGetError() != GL_NO_ERROR)
    {
        timeout--;
        if (!timeout) 
        {
            std::cout << "GLClearError: Could not clear all errors" << std::endl;
            break;
        }
    }
}

bool GLCheckError()
{
    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR)
    {
        std::cout << "OpenGL reports error code 0x" << std::hex << errorCode << std::dec;
        return false;
    }
    return true;
}

void GLFWClearError()
{
    uint timeout = MAX_CLEAR_ERROR_CALLS;
    char description[1024];

    while (glfwGetError((const char**)(&description)) != GLFW_NO_ERROR)
    {
        timeout--;
        if (!timeout) 
        {
            std::cout << "GLFWClearError: Could not clear all errors" << std::endl;
            break;
        }
    }
}

bool GLFWCheckError()
{
    int errorCode = glGetError();
    if (errorCode != GLFW_NO_ERROR)
    {
        std::cout << "GLFW reports error code 0x" << std::hex << errorCode << std::dec;
        return false;
    }
    return true;
}
