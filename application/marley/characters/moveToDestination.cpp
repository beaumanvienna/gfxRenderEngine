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

#include "engine.h"
#include "marley/marley.h"
#include "marley/characters/moveToDestination.h"

namespace MarleyApp
{
    MoveToDestination::MoveToDestination()
        : m_Activated(false),
          m_Destination(glm::vec2{0.0f, 0.0f})
    {}

    void MoveToDestination::SetDestination(float x, float y)
    {
        m_Activated = true;
        m_Destination = glm::vec2{x, y};
    }
    
    void MoveToDestination::GetMovement(glm::vec2& movementCommand)
    {
        if (m_Activated)
        {
            LOG_APP_CRITICAL("yes move");
            movementCommand += glm::vec2{-1.0f, 0.0f};
        } else {
            LOG_APP_CRITICAL("no move");}
    }
}
