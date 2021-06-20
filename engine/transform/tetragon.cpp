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

#include "tetragon.h"
#include "gtc/matrix_transform.hpp"
#include "matrix.h"
#include "core.h"

Tetragon::Tetragon(const glm::vec2& leftBottom, const glm::vec2& rightBottom, const glm::vec2 rightTop, const glm::vec2 leftTop, const float scale)
    : m_LeftBottom(leftBottom), m_RightBottom(rightBottom), m_RightTop(rightTop), m_LeftTop(leftTop), m_ScaleX(scale), m_ScaleY(scale)
{
    SetScaleMatrix();
}

Tetragon::Tetragon(const glm::vec2& leftBottom, const glm::vec2& rightBottom, const glm::vec2 rightTop, const glm::vec2 leftTop, const float scaleX, const float scaleY)
    : m_LeftBottom(leftBottom), m_RightBottom(rightBottom), m_RightTop(rightTop), m_LeftTop(leftTop), m_ScaleX(scaleX), m_ScaleY(scaleY)
{
    SetScaleMatrix();
}

void Tetragon::SetScaleMatrix(const float scale)
{
    m_ScaleX = m_ScaleY = scale;
    SetScaleMatrix();
}

void Tetragon::SetScaleMatrix(const float scaleX, const float scaleY)
{
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;
    SetScaleMatrix();
}

void Tetragon::SetScaleMatrix()
{
    glm::mat4 positionMatrix = glm::mat4
    (
        m_LeftBottom.x,  m_LeftBottom.y,  1.0f, 1.0f,
        m_RightBottom.x, m_RightBottom.y, 1.0f, 1.0f,
        m_RightTop.x,    m_RightTop.y,    1.0f, 1.0f,
        m_LeftTop.x,     m_LeftTop.y,     1.0f, 1.0f
    );
    
    // model matrix
    glm::vec3 scaleVec(m_ScaleX, m_ScaleY,0.0f);
    m_ScaleMatrix = Scale(scaleVec) * positionMatrix;
}

bool Tetragon::IsInBounds(const glm::vec2& position) const
{
    bool left = false, right = false, top = false, bottom = false, isInBounds = false;
    
    // left 
    float deltaLeftY = m_LeftTop.y - m_LeftBottom.y;
    if (deltaLeftY == 0) return false;
    float deltaLeftX = m_LeftTop.x - m_LeftBottom.x;
    float relativeLeftDeltaY = (position.y-m_LeftBottom.y)/deltaLeftY;
    float leftBoundX = m_LeftBottom.x + deltaLeftX * relativeLeftDeltaY;
    if (relativeLeftDeltaY <= 0.0f)
    {
        left = position.x > m_LeftBottom.x;
    }
    else if (relativeLeftDeltaY <= 1.0f)
    {
        left = position.x > leftBoundX;
    }
    else
    {
        left = position.x > m_LeftTop.x;
    }
    
    // right
    float deltaRightY = m_RightTop.y - m_RightBottom.y;
    if (deltaRightY == 0) return false;
    float deltaRightX = m_RightTop.x - m_RightBottom.x;
    float relativeRightDeltaY = (position.y-m_RightBottom.y)/deltaRightY;
    float rightBoundX = m_RightBottom.x + deltaRightX * relativeRightDeltaY;
    if (relativeRightDeltaY <= 0.0f)
    {
        right = position.x < m_RightBottom.x;
    }
    else if (relativeRightDeltaY <= 1.0f)
    {
        right = position.x < rightBoundX;
    }
    else
    {
        right = position.x < m_RightTop.x;
    }
    
    // top
    float deltaTopX = m_RightTop.x - m_LeftTop.x;
    if (deltaTopX == 0) return false;
    float deltaTopY = m_RightTop.y - m_LeftTop.y;
    float relativeTopDeltaX = (position.x-m_LeftTop.x)/deltaTopX;
    float topBoundY = m_LeftTop.y + deltaTopY * relativeTopDeltaX;
    if (relativeTopDeltaX <= 0.0f)
    {
        top = position.y < m_LeftTop.y;
    }
    else if (relativeTopDeltaX <= 1.0f)
    {
        top = position.y < topBoundY;
    }
    else
    {
        top = position.y < m_RightTop.y;
    }
    
    // bottom
    float deltaBottomX = m_RightTop.x - m_LeftTop.x;
    if (deltaBottomX == 0) return false;
    float deltaBottomY = m_RightBottom.y - m_LeftBottom.y;
    float relativeBottomDeltaX = (position.x-m_LeftBottom.x)/deltaBottomX;
    float bottomBoundY = m_LeftBottom.y + deltaBottomY * relativeBottomDeltaX;
    if (relativeBottomDeltaX <= 0.0f)
    {
        bottom = position.y > m_LeftBottom.y;
    }
    else if (relativeBottomDeltaX <= 1.0f)
    {
        bottom = position.y > bottomBoundY;
    }
    else
    {
        bottom = position.y > m_RightBottom.y;
    }

    isInBounds = left && right && top && bottom;
    
    return isInBounds;
}
