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

#include "marley/gameState.h"
#include "marley/characters/characterEvent.h"
#include "marley/emulation/emulationEvent.h"
#include "marley/marley.h"

namespace MarleyApp
{

    void GameState::Start()
    {
        m_Translation = glm::vec3(SPLASHSCREEN_SPWANPOINT.x, SPLASHSCREEN_SPWANPOINT.y, 0.0f);

        m_WalkAreaSplash = new Tetragon(
                                        {   22.0f, -439.5f},  // leftBottom
                                        { 1100.0f, -436.5f},  // rightBottom
                                        { 1100.0f, -430.0f},  // rightTop
                                        {   22.0f, -430.0f}); // leftTop
        m_WalkAreaMain = new Tetragon(
                                        { 211.5f, -439.5f},  // leftBottom
                                        { 901.5f, -436.5f},  // rightBottom
                                        { 891.5f, -226.0f},  // rightTop
                                        {-153.0f, -102.0f}); // leftTop
    }

    void GameState::Shutdown()
    {
        if (m_WalkAreaSplash)
        {
            delete m_WalkAreaSplash;
            m_WalkAreaSplash = nullptr;
        }
        if (m_WalkAreaMain)
        {
            delete m_WalkAreaMain;
            m_WalkAreaMain = nullptr;
        }
        if (EmulationIsRunning())
        {
            ExitEmulation();
            SetEmulationMode(OFF);
        }
    }

    void GameState::OnUpdate()
    {
        switch(m_Scene)
        {
            case Scene::SPLASH:
            {
                if (m_Translation.x>1000.0f)
                {
                    m_Scene = MAIN;
                    m_Translation.x = MAINSCREEN_SPWANPOINT.x;
                }
                else if (m_Translation.x>870.0f)
                {
                    Marley::m_Application->SetDestination(glm::vec2(1100.0f, SPLASHSCREEN_SPWANPOINT.y));
                }
                break;
            }
            case Scene::MAIN:
            {
                break;
            }
        }
    }

    void GameState::OnEvent(Event& event)  {}

    Tetragon* GameState::GetWalkArea() const
    {
        Tetragon* walkArea;
        switch(m_Scene)
        {
            case Scene::SPLASH:
                walkArea = m_WalkAreaSplash;
                break;
            case Scene::MAIN:
                walkArea = m_WalkAreaMain;
                break;
            default:
                walkArea = m_WalkAreaSplash;
         }
         return walkArea;
    }

    void GameState::ExitEmulation()
    {
        EmulatorQuitEvent quitEvent;
        m_EventCallback(quitEvent);
    }

    void GameState::Load()
    {
        EmulatorLoadEvent loadEvent;
        m_EventCallback(loadEvent);
    }

    void GameState::Save()
    {
        EmulatorSaveEvent saveEvent;
        m_EventCallback(saveEvent);
    }

    void GameState::InputIdle(bool isIdle)
    {
        if (m_InputIdle != isIdle)
        {
            InputIdleEvent inputIdleEvent(isIdle);
            m_EventCallback(inputIdleEvent);
            m_InputIdle = isIdle;
        }
    }

    void GameState::SetEventCallback(const AppEventCallbackFunction& callback)
    {
        m_EventCallback = callback;
    }
    void GameState::PrintState()
    {
        switch(m_EmulationMode)
        {
            case RUNNING:
                LOG_APP_INFO("game state running");
                break;
            case PAUSED:
                LOG_APP_INFO("game state paused");
                break;
            case OFF:
                LOG_APP_INFO("game state off");
                break;
        }
    }
    void GameState::SetEmulationMode(EmulationMode mode)
    {
        m_EmulationMode = mode;
        switch(m_EmulationMode)
        {
            case RUNNING:
                Engine::m_Engine->DisallowCursor();
                break;
            case PAUSED:
                Engine::m_Engine->AllowCursor();
                break;
            case OFF:
                Engine::m_Engine->AllowCursor();
                break;
        }
    }
    
    void GameState::EnableUserInput(bool enable)
    {
        m_UserInputEnabled = enable;
    }
}
