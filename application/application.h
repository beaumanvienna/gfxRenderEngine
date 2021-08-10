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

#include "engine.h"
#include "mainScreenBackground.h"
#include "GuybrushWalk.h"
#include "splash.h"
#include "imguiOverlay.h"
#include "gameState.h"
#include "tetragon.h"
#include "glm.hpp"
#include "UI.h"
#include "UIControllerIcon.h"
#include "messageBoard.h"
#include "splashLogo.h"
#include "tilemapLayer.h"
#include "appSettings.h"
#include "stars.h"
#include "cursor.h"

class Application : public EngineApp
{
    
public:

    bool Start();
    void Shutdown();
    void OnUpdate();
    void OnEvent(Event& event);
    void OnResize();
    void OnScroll();
    void InitSettings();
    void InitCursor();
    void Flush();
    
    static Application* m_Application;
    static std::unique_ptr<GameState> m_GameState;
    
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

    bool m_EnableImgui;
    
    AppSettings m_AppSettings{&Engine::m_SettingsManager};

};
