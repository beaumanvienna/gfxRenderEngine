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

#include <iostream>

#include "engine.h"
#include "UIscreen.h"
#include "viewGroup.h"
#include "infoMessage.h"
#include "marley/UI/browser/ROMbrowser.h"

namespace MarleyApp
{

    class MainScreen : public SCREEN_UIDialogScreen
    {
    public:
        MainScreen(SpriteSheet* spritesheet) { m_SpritesheetMarley = spritesheet; }
        virtual ~MainScreen() {}
        bool key(const SCREEN_KeyInput &key) override;
        void OnAttach();
        void OnDetach();
        void update() override;
        void onFinish(DialogResult result) override;
        std::string tag() const override { return "main screen"; }

    protected:
        void CreateViews() override;

        SCREEN_UI::EventReturn settingsClick(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn offClick(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn offHold(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn HomeClick(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn OnROMBrowserHoldChoice(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn OnROMClick(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn OnROMBrowserNavigateClick(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn EmulationPaused(SCREEN_UI::EventParams &e);

    private:

        SCREEN_UI::Choice* m_OffButton;
        SCREEN_UI::ScrollView* m_GameLauncherFrameScroll;
        ROMBrowser *m_ROMbrowser;
        std::string m_LastGamePath;

        enum toolTipID
        {
            MAIN_HOME = 0,
            MAIN_SETTINGS,
            MAIN_OFF,
            SETTINGS_HOME,
            SETTINGS_GRID,
            SETTINGS_LINES,
            MAX_TOOLTIP_IDs
        };
        InfoMessage *m_MainInfo;
        bool m_ToolTipsShown[MAX_TOOLTIP_IDs] = {0,0,0,0,0,0};
        SCREEN_UI::TextView* m_GamesPathView;

        SpriteSheet* m_SpritesheetMarley;
        SpriteSheet m_SpritesheetSettings;
        SpriteSheet m_SpritesheetOff;
        SpriteSheet m_SpritesheetHome;
        SpriteSheet m_SpritesheetLines;
        SpriteSheet m_SpritesheetGrid;

    };
}
