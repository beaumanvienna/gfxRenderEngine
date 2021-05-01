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

#include "engine.h"
#include "platform.h"

class VertexBuffer
{
    
public:

    // create verticies
    /* positions
     * ( -0.5f,  0.5f) (  0.0f,  0.5f) (  0.5f,  0.5f)
     * ( -0.5f,  0.0f) (  0.0f,  0.0f) (  0.5f,  0.0f)
     * ( -0.5f, -0.5f) (  0.0f, -0.5f) (  0.5f, -0.5f)
     * 
    */

    /* texture coordinates
     * (  0.0f,  1.0f) (  0.5f,  1.0f) (  1.0f,  1.0f)
     * (  0.0f,  0.5f) (  0.5f,  0.5f) (  1.0f,  5.0f)
     * (  0.0f,  0.0f) (  0.5f,  0.0f) (  1.0f,  0.0f)
     * 
    */
    
    struct Vertex
    {
        float m_Position[2]; // 2D
        float m_TextureCoordinates[2]; 
        float m_Index;
    };
    

    VertexBuffer(uint size);
    ~VertexBuffer();
    
    void LoadBuffer(const void* verticies, uint size);
    void BeginDrawCall() { m_BufferOffset = 0; }
    void Bind() const;
    void Unbind() const;
    
private: 

    uint m_RendererID;
    uint m_BufferOffset;

};
