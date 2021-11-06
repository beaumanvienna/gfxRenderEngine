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

#include <chrono>
#include <cmath>

#include "engine.h"
#include "marley/marley.h"
#include "marley/characters/moveToDestination.h"

namespace MarleyApp
{
    MoveToDestination::MoveToDestination()
        : m_Activated(false), m_Count(0),
          m_Destination(glm::vec2{0.0f, 0.0f})
    {
        m_CurrentPosition = Marley::m_GameState->GetHeroPosition();
    }

    void MoveToDestination::SetDestination(float x, float y)
    {
        m_Activated = true;
        m_Destination = glm::vec2{x, y};
        m_StartPosition = *m_CurrentPosition;
        
        // direction
        float dirX = m_Destination.x - m_CurrentPosition->x;
        float dirY = m_Destination.y - m_CurrentPosition->y;
        m_Direction = glm::vec2{dirX, dirY};
        m_OldPosition = glm::vec2{0.0f, 0.0f};
        m_Count = 0;
    }

    void MoveToDestination::GetMovement(glm::vec2& movementCommand)
    {
        if (m_Activated)
        {

            float deltaX = m_Destination.x - m_CurrentPosition->x;
            float deltaY = m_Destination.y - m_CurrentPosition->y;

            float moveX = ( (std::signbit(deltaX) == std::signbit(m_Direction.x)) ? m_Direction.x : 0.0f);
            float moveY = ( (std::signbit(deltaY) == std::signbit(m_Direction.y)) ? m_Direction.y : 0.0f);

            movementCommand += glm::vec2{moveX, moveY};
            
            bool destinationUnreachable = (m_OldPosition.x == deltaX) && (m_OldPosition.y == deltaY);
            if (destinationUnreachable)
            {
                m_Count++;
            }
            else
            {
                m_Count = 0;
            }
            if (m_Count > 60) ResetDestination();
            
            m_OldPosition.x = deltaX;
            m_OldPosition.y = deltaY;
        }
    }
}
