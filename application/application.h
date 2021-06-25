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
#include "mainScreen.h"
#include "GuybrushWalk.h"
#include "splash.h"
#include "imguiOverlay.h"
#include "gameState.h"
#include "tetragon.h"
#include "glm.hpp"
#include "UIController.h"

class Application : public EngineApp
{
    
public:

    bool Start();
    void Shutdown();
    void OnUpdate();
    void OnEvent(Event& event);
    void OnResize();
    void OnScroll();
    
    SpriteSheet m_SpritesheetMarley;
    static std::unique_ptr<GameState> m_GameState;

private:

    // layers
    Splash*          m_Splash        = nullptr;
    MainScreenLayer* m_MainScreen    = nullptr;
    Overlay*         m_Overlay       = nullptr;
    UIController*    m_UIController  = nullptr;
    ImguiOverlay*    m_ImguiOverlay  = nullptr;
    
    bool m_EnableImgui;

};
