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
#include "layer.h"
#include "indexBuffer.h"
#include "vertexBuffer.h"
#include "spritesheet.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "event.h"
#include "core.h"
#include "controllerEvent.h"

class Overlay : public Layer
{
    
public:

    Overlay(IndexBuffer* indexBuffer, VertexBuffer* vertexBuffer, const std::string& name = "layer")
        : Layer(name), m_IndexBuffer(indexBuffer), m_VertexBuffer(vertexBuffer)
    {
    }
    
    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Event& event) override;
    void OnUpdate() override;
    
    // event processing
    void OnControllerButtonPressed(ControllerButtonPressedEvent& event);
    void OnControllerButtonReleased(ControllerButtonReleasedEvent& event);
    
private:
    IndexBuffer*  m_IndexBuffer;
    VertexBuffer* m_VertexBuffer;
    
    // sprite sheets
    SpriteSheet m_SpritesheetHorn;
    SpriteAnimation* m_HornAnimation;
    
    float normalizeX;
    float normalizeY;
    
    float scaleTextureX;
    float scaleTextureY;

    float m_ScaleMainWindowAspectRatio;

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
    
};
