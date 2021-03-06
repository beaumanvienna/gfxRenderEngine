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

#include "glm.hpp"
#include "marley/characters/moveToDestination.h"

// 1.) automatically move charater (for example on the splash screen after a delay)
// 2.) move to a destination (for example requested by mouse click)

namespace MarleyApp
{

    class AutoMove
    {

    public:
        enum AutoMoveType
        {
            MOVE_TO_DESTINATION,
            MOVE_APP_CONTROLLED
        };

        AutoMove();

        void SetActivationState(bool activate);
        void GetMovement(AutoMoveType movementType, glm::vec2& movementCommand);

        void SetDestination(const glm::vec2& destination) { m_MoveToDestination.SetDestination(destination); }

    private:

        void GetMovementAppControlled(glm::vec2& movementCommand);
        void GetMovementToDestination(glm::vec2& movementCommand);

        bool m_Activated;
        MoveToDestination m_MoveToDestination;

    };
}
