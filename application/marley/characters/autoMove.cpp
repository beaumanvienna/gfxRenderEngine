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
#include "marley/characters/autoMove.h"

namespace MarleyApp
{
    AutoMove::AutoMove()
        : m_Activated(false)
    {}

    void AutoMove::GetMovement(glm::vec2& movementCommand)
    {
        if (m_Activated)
        {
            GameState::Scene scene = Marley::m_GameState->GetScene();
            GameState::EmulationMode emulationMode = Marley::m_GameState->GetEmulationMode();

            switch(scene)
            {
                case GameState::SPLASH:
                    {
                        constexpr auto SPLASH_IDLE_TIME = 15s;
                        auto splashStartTime = Marley::m_Application->GetSplashStartTime();
                        auto now = std::chrono::steady_clock::now();

                        if ((now - splashStartTime) > SPLASH_IDLE_TIME)
                        {
                            movementCommand += glm::vec2{1.0, 0.0f};
                        }
                    }
                    break;
                case GameState::MAIN:
                    break;
            }

        }
    }
}
