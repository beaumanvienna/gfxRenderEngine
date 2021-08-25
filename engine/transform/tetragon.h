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
#include "glm.hpp"

class Tetragon
{

public:

    Tetragon(const glm::vec2& leftBottom, const glm::vec2& rightBottom, const glm::vec2 rightTop, const glm::vec2 leftTop, const float scale = 1.0f);
    Tetragon(const glm::vec2& leftBottom, const glm::vec2& rightBottom, const glm::vec2 rightTop, const glm::vec2 leftTop, const float scaleX, const float scaleY);

    void SetScaleMatrix(const float scale);
    void SetScaleMatrix(const float scaleX, const float scaleY);
    const glm::mat4& GetScaleMatrix() const { return m_ScaleMatrix; }
    bool MoveInArea(glm::vec3* translation, const glm::vec2& movement);

private:

    bool IsInBounds(const glm::vec2& position);
    void SetScaleMatrix();

private:

    glm::vec2 m_LeftBottom;
    glm::vec2 m_RightBottom;
    glm::vec2 m_RightTop;
    glm::vec2 m_LeftTop;

    float m_ScaleX;
    float m_ScaleY;
    glm::mat4 m_ScaleMatrix;
    
    bool m_Left, m_Right, m_Top, m_Bottom;
    float m_LeftBoundX, m_RightBoundX, m_TopBoundY, m_BottomBoundY;
    
    float m_DeltaLeftY;
    float m_DeltaLeftX;
    float m_DeltaRightY;
    float m_DeltaRightX;
    float m_DeltaTopX;
    float m_DeltaTopY;
    float m_DeltaBottomX;
    float m_DeltaBottomY;

};
