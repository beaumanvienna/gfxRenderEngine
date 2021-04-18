/* Engine Copyright (c) 2021 Engine Development Team 
   https://github.com/beaumanvienna/marley

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

#include <iostream>
#include "engine.h"
#include "platform.h"
#include "GLFW/glfw3.h"

int main(int argc, char* argv[])
{
    std::cout << "OpenGL test program" << std::endl;
    
    GLFWwindow* gWindow;
    
    if (!glfwInit())
    {
        std::cout << "glfwInit() failed" << std::endl;
        return -1;
    }
    
    gWindow = glfwCreateWindow(640,480,"Engine",NULL,NULL);
    
    if (!gWindow)
    {
        glfwTerminate();
        std::cout << "Failed to create main window" << std::endl;
        return -1;
    }
    
    glfwMakeContextCurrent(gWindow);
    
    while (!glfwWindowShouldClose(gWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        /*
         * ( -0.5f,  0.5f) (  0.0f,  0.5f) (  0.5f,  0.5f)
         * ( -0.5f,  0.0f) (  0.0f,  0.0f) (  0.5f,  0.0f)
         * ( -0.5f, -0.5f) (  0.0f, -0.5f) (  0.5f, -0.5f)
         * 
        */
        glBegin(GL_TRIANGLES);
        glVertex2f( -0.5f, -0.5f);
        glVertex2f(  0.5f, -0.5f);
        glVertex2f(  0.0f,  0.5f);
        glEnd();
        
        glfwSwapBuffers(gWindow);
        
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
};
