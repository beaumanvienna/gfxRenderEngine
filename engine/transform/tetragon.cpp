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
    
    m_DeltaLeftY = m_LeftTop.y - m_LeftBottom.y;
    m_DeltaLeftX = m_LeftTop.x - m_LeftBottom.x;
    m_DeltaRightY = m_RightTop.y - m_RightBottom.y;
    m_DeltaRightX = m_RightTop.x - m_RightBottom.x;
    m_DeltaTopX = m_RightTop.x - m_LeftTop.x;
    m_DeltaTopY = m_RightTop.y - m_LeftTop.y;
    m_DeltaBottomX = m_RightTop.x - m_LeftTop.x;
    m_DeltaBottomY = m_RightBottom.y - m_LeftBottom.y;
    
}

bool Tetragon::IsInBounds(const glm::vec2& position)
{
    if (m_DeltaLeftY == 0)   return false;
    if (m_DeltaRightY == 0)  return false;
    if (m_DeltaTopX == 0)    return false;
    if (m_DeltaBottomX == 0) return false;

    bool isInBounds = false;
    m_Left = m_Right = m_Top = m_Bottom = false;

    // left 
    float relativeLeftDeltaY = (position.y-m_LeftBottom.y)/m_DeltaLeftY;
    m_LeftBoundX = m_LeftBottom.x + m_DeltaLeftX * relativeLeftDeltaY;
    if (relativeLeftDeltaY <= 0.0f)
    {
        m_Left = position.x >= m_LeftBottom.x;
    }
    else if (relativeLeftDeltaY <= 1.0f)
    {
        m_Left = position.x >= m_LeftBoundX;
    }
    else
    {
        m_Left = position.x >= m_LeftTop.x;
    }

    // right
    float relativeRightDeltaY = (position.y-m_RightBottom.y)/m_DeltaRightY;
    m_RightBoundX = m_RightBottom.x + m_DeltaRightX * relativeRightDeltaY;
    if (relativeRightDeltaY <= 0.0f)
    {
        m_Right = position.x <= m_RightBottom.x;
    }
    else if (relativeRightDeltaY <= 1.0f)
    {
        m_Right = position.x <= m_RightBoundX;
    }
    else
    {
        m_Right = position.x <= m_RightTop.x;
    }

    // top
    float relativeTopDeltaX = (position.x-m_LeftTop.x)/m_DeltaTopX;
    m_TopBoundY = m_LeftTop.y + m_DeltaTopY * relativeTopDeltaX;
    if (relativeTopDeltaX <= 0.0f)
    {
        m_Top = position.y <= m_LeftTop.y;
    }
    else if (relativeTopDeltaX <= 1.0f)
    {
        m_Top = position.y <= m_TopBoundY;
    }
    else
    {
        m_Top = position.y <= m_RightTop.y;
    }
    
    // bottom
    float relativeBottomDeltaX = (position.x-m_LeftBottom.x)/m_DeltaBottomX;
    m_BottomBoundY = m_LeftBottom.y + m_DeltaBottomY * relativeBottomDeltaX;
    if (relativeBottomDeltaX <= 0.0f)
    {
        m_Bottom = position.y >= m_LeftBottom.y;
    }
    else if (relativeBottomDeltaX <= 1.0f)
    {
        m_Bottom = position.y >= m_BottomBoundY;
    }
    else
    {
        m_Bottom = position.y >= m_RightBottom.y;
    }

    isInBounds = m_Left && m_Right && m_Top && m_Bottom;
    
    return isInBounds;
}

bool Tetragon::MoveInArea(glm::vec3* translation, const glm::vec2& movement)
{
    glm::vec2 position = glm::vec2(translation->x+movement.x,translation->y+movement.y);
    if (IsInBounds(position))
    {
        translation->x = position.x;
        translation->y = position.y;
        return true;
    }
    
    // try to find alternative position
    bool isInBounds = false;
    
    if (!m_Left)
    {
        if (movement.x < 0.0f) return false;
        position.x = m_LeftBoundX;
    }
    else if (!m_Right)
    {
        if (movement.x > 0.0f) return false;
        position.x = m_RightBoundX;
    }
    else if (!m_Top)
    {
        if (movement.y > 0.0f) return false;
        position.y = m_TopBoundY;
    }
    else if (!m_Bottom)
    {
        if (movement.y < 0.0f) return false;
        position.y = m_BottomBoundY;
    }

    if (IsInBounds(position))
    {
        isInBounds = true;
        translation->x = position.x;
        translation->y = position.y;
    }

    return isInBounds;
}
