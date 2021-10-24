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

#define APP_INCLUDE 1

#include <memory>
#include <iostream>

#include "engine.h"
#include "application.h"
#include "marley/mainScreenBackground.h"
#include "marley/characters/GuybrushWalk.h"
#include "marley/splash/splash.h"
#include "marley/imguiOverlay.h"
#include "marley/gameState.h"
#include "tetragon.h"
#include "glm.hpp"
#include "marley/UI/UI.h"
#include "marley/UI/UIControllerIcon.h"
#include "marley/UI/messageBoard.h"
#include "marley/splash/splashLogo.h"
#include "marley/testing/tilemapLayer.h"
#include "marley/emulation/emulationUtils.h"
#include "marley/emulation/emulatorLayer.h"
#include "marley/emulation/instructions.h"
#include "marley/emulation/bios.h"
#include "marley/appSettings.h"
#include "marley/UI/stars.h"
#include "cursor.h"

namespace MarleyApp
{

    class Marley : public Application
    {

    public:

        virtual bool Start() override;
        virtual void Shutdown() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(Event& event) override;
        virtual void Flush() override;
        virtual std::string GetConfigFilePath() override;
        void OnAppEvent(AppEvent& event);

        void OnResize();
        void OnScroll();
        void InitCursor();
        void InitSettings();

        static Marley* m_Application;
        static std::unique_ptr<GameState> m_GameState;
        static std::unique_ptr<EmulationUtils> m_EmulationUtils;
        static Bios m_BiosFiles;

        std::shared_ptr<Cursor> m_Cursor;
        SpriteSheet m_SpritesheetMarley;

    private:

        // layers
        Splash*                 m_Splash               = nullptr;
        MainScreenBackground*   m_MainScreenBackground = nullptr;
        Overlay*                m_Overlay              = nullptr;
        UIControllerIcon*       m_UIControllerIcon     = nullptr;
        UI*                     m_UI                   = nullptr;
        SplashLogo*             m_SplashLogo           = nullptr;
        ImguiOverlay*           m_ImguiOverlay         = nullptr;
        TilemapLayer*           m_TilemapLayer         = nullptr;
        MessageBoard*           m_MessageBoard         = nullptr;
        UIStarIcon*             m_UIStarIcon           = nullptr;
        EmulatorLayer*          m_EmulatorLayer        = nullptr;
        Instructions*           m_Instructions         = nullptr;

        bool m_EnableImgui;

        AppSettings m_AppSettings{&Engine::m_SettingsManager};

    };
}
