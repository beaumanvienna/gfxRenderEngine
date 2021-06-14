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

#include "GLrendererAPI.h"
#include "GL.h"


void GLRendererAPI::SetClearColor(const glm::vec4& color)
{
    GLCall(glClearColor(color.r, color.g, color.b, color.a));
}

void GLRendererAPI::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void GLRendererAPI::EnableBlending() const
{
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void GLRendererAPI::DisableBlending() const
{
    GLCall(glDisable(GL_BLEND));
}

void GLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) const
{
    auto indexBuffers = vertexArray->GetIndexBuffers();

    // bind & write index buffer
    indexBuffers[0]->EndScene();

    // the actual draw call
    GLCall(glDrawElements
    (
        GL_TRIANGLES,                 /* mode */
        indexBuffers[0]->GetCount(),  /* count */
        GL_UNSIGNED_INT,              /* type */
        (void*)0                      /* element array buffer offset */
    ));
}
    
