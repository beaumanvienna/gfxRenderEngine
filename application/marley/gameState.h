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
#include "tetragon.h"
#include "marley/appEvent.h"

namespace MarleyApp
{

    class GameState
    {

    public:

        enum Scene
        {
            SPLASH,
            MAIN,
            SETTINGS
        };

        enum EmulationMode
        {
            OFF,
            RUNNING,
            PAUSED,
            PREVIEW
        };

    public:

        GameState()
            : m_Scene(SPLASH), m_EmulationMode(OFF),
              m_InputIdle(false) {}

        void Start();
        void Shutdown();
        void OnEvent(Event& event);
        void OnUpdate();

        void SetEventCallback(const AppEventCallbackFunction& callback);
        void SetEmulationMode(EmulationMode mode);
        EmulationMode GetEmulationMode() const { return m_EmulationMode; }
        bool EmulationIsRunning() const { return m_EmulationMode == RUNNING; }
        bool EmulationIsPaused() const { return m_EmulationMode == PAUSED; }
        bool EmulationIsNotRunning() const { return m_EmulationMode == OFF; }
        void Load();
        void Save();
        Scene GetScene() const { return m_Scene; }
        Tetragon* GetWalkArea() const;
        glm::vec3* GetHeroPosition() { return &m_Translation; }
        void ExitEmulation();
        void PrintState();
        void InputIdle(bool isIdle);

    private:

        Scene m_Scene;
        EmulationMode m_EmulationMode;
        AppEventCallbackFunction m_EventCallback;

        glm::vec3 m_Translation;

        Tetragon* m_WalkAreaSplash;
        Tetragon* m_WalkAreaMain;
        
        bool m_InputIdle;

    };
}
