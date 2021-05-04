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

#include "core.h"

#include "log.h"
#include "shader.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "renderer.h"
#include "texture.h"
#include "spritesheet.h"

class EngineApp
{
    
public:

    EngineApp();
    virtual ~EngineApp();
    
    bool Init(Engine* engine);
    virtual void Run() = 0;

protected:

    Engine* m_Engine;

    float normalizeX;
    float normalizeY;
    
    const uint NUMBER_OF_VERTICIES = 1024;
    VertexBuffer vertexBuffer;
    
    //create vertex array object (vao)
    VertexArray vertexArray;
    
    VertexBufferLayout vertexBufferLayout;
    
    //create empty index buffer object (ibo)
    IndexBuffer indexBuffer;

    ShaderProgram shaderProg;
    Renderer renderer;
    
    float scaleTextureX;
    float scaleTextureY;

    float scaleMainWindowAspectRatio;

    float scaleSize;
    float scaleResolution;

    float ortho_left;
    float ortho_right;
    float ortho_bottom;
    float ortho_top;
    float ortho_near;
    float ortho_far;

    float orthoLeft;
    float orthoRight;
    float orthoBottom;
    float orthoTop;
    
    float pos1X;
    float pos1Y; 
    float pos2X; 
    float pos2Y;

    glm::mat4 normalizedPosition;
    
    Sprite* sprite;
    glm::vec4 position1;
    glm::vec4 position2;
    glm::vec4 position3;
    glm::vec4 position4;
    
private:
    
};
