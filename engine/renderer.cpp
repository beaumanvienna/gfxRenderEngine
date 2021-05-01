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

#include "renderer.h"
#include "OpenGL/GL.h"
#include <unistd.h>

Renderer::Renderer(GLFWwindowPtr window) : m_Window(window)
{
    // set the number of screen updates to wait from the time glfwSwapBuffers 
    // was called before swapping the buffers
    GLCall(glfwSwapInterval(1)); // wait for next screen update
}

Renderer::~Renderer()
{

}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
    
void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shaderProg) const
{    
    // enable buffers and shaders
    vertexArray.Bind();
    indexBuffer.Bind();
    shaderProg.Bind();
    
    GLCall(glDrawElements
    (
        GL_TRIANGLES,                                           /* mode */
        indexBuffer.GetCount(),                                 /* count */
        GL_UNSIGNED_INT,                                        /* type */
        (void*)0                                                /* element array buffer offset */
    ));
}

void Renderer::SwapBuffers() const
{
    usleep(32000); // ~30 frames per second (in micro (!) seconds)
    GLCall(glfwSwapBuffers(m_Window));
}

void Renderer::EnableBlending() const
{
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}
void Renderer::DisableBlending() const
{
    GLCall(glDisable(GL_BLEND));
}