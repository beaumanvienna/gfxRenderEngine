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

#include "engine.h"
#include "platform.h"
#include "core.h"
#include "engineApp.h"
#include "log.h"

EngineApp::EngineApp()
{
}

EngineApp::~EngineApp()
{
}

bool EngineApp::Init(Engine* engine)
{
    m_Engine = engine;
    // --- model, view, projection matrix ---
        
    // projection matrix
    // orthographic matrix for projecting two-dimensional coordinates onto the screen

    // normalize to -0.5f - 0.5f
    normalizeX = 0.5f;
    normalizeY = 0.5f;
    
    //create empty vertex buffer object (vbo)
    vertexBuffer.Create(sizeof(VertexBuffer::Vertex) * NUMBER_OF_VERTICIES);

    // push position floats into attribute layout
    vertexBufferLayout.Push<float>(member_size(VertexBuffer::Vertex,m_Position)/sizeof(float));
    
    // push texture coordinates floats into attribute layout
    vertexBufferLayout.Push<float>(member_size(VertexBuffer::Vertex,m_TextureCoordinates)/sizeof(float));

    // push texture index float into attribute layout
    vertexBufferLayout.Push<float>(member_size(VertexBuffer::Vertex,m_Index)/sizeof(float));

    vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
    
    // program the GPU
    shaderProg.AddShader(GL_VERTEX_SHADER,   "engine/shader/vertexShader.vert");
    shaderProg.AddShader(GL_FRAGMENT_SHADER, "engine/shader/fragmentShader.frag");
    shaderProg.Create();
    
    if (!shaderProg.IsOK())
    {
        std::cout << "Shader creation failed" << std::endl;
        return false;
    }
    
    const uint TEXTURE_SLOT_0 = 0;
    int textureIDs[8] = 
    {
        TEXTURE_SLOT_0 + 0, TEXTURE_SLOT_0 + 1, TEXTURE_SLOT_0 + 2, TEXTURE_SLOT_0 + 3,
        TEXTURE_SLOT_0 + 4, TEXTURE_SLOT_0 + 5, TEXTURE_SLOT_0 + 6, TEXTURE_SLOT_0 + 7
    };
    shaderProg.setUniform1iv("u_Textures", 4, textureIDs);
    
    // create Renderer
    renderer.Create((GLFWwindow*)m_Engine->GetWindow());
    renderer.EnableBlending();

    // detach everything
    vertexBuffer.Unbind();
    vertexArray.Unbind();
    indexBuffer.Unbind();
    shaderProg.Unbind();

    // aspect ratio of image
    scaleTextureX = 1.0f;

    // aspect ratio of main window 
    scaleMainWindowAspectRatio = m_Engine->GetWindowAspectRatio();

    // scale it to always have the same physical size on the screen
    // independently of the resolution
    scaleResolution = 1.0f / m_Engine->GetWindowScale();

    ortho_left   =-normalizeX * scaleResolution;
    ortho_right  = normalizeX * scaleResolution;
    ortho_bottom =-normalizeY * scaleResolution * scaleMainWindowAspectRatio;
    ortho_top    = normalizeY * scaleResolution * scaleMainWindowAspectRatio;
    ortho_near   =  1.0f;
    ortho_far    = -1.0f;

    normalizedPosition = glm::mat4
    (
        -0.5f,  0.5f, 1.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 1.0f, 1.0f
    );
    
    return true;

}
