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
#include "UI/browser/directoryBrowser.h"
#include "controllerSetup.h"

inline constexpr float TAB_SCALE = 1.5f;

class SettingsScreen : public SCREEN_UIDialogScreen
{

public:

    SettingsScreen(SpriteSheet* spritesheet) { m_SpritesheetMarley = spritesheet; }
    virtual ~SettingsScreen() { m_IsCreditsScreen = false; }
    bool key(const SCREEN_KeyInput &key) override;
    void OnAttach();
    void update() override;
    void onFinish(DialogResult result) override;
    std::string tag() const override { return "settings screen"; }
    static bool m_IsCreditsScreen;

protected:
    void CreateViews() override;
    
private:
    enum
    {
        SEARCH_SCREEN,
        CONTROLLER_SETUP,
        DOLPHIN_SCREEN,
        PCSX2_SCREEN,
        GENERAL_SCREEN,
        CREDITS_SCREEN
    };
    
private:

    SCREEN_UI::EventReturn OnFullscreenToggle(SCREEN_UI::EventParams &e);
    SCREEN_UI::EventReturn OnThemeChanged(SCREEN_UI::EventParams &e);
    SCREEN_UI::TextView*   BiosInfo(std::string infoText, bool biosFound);
    ControllerSetup* m_ControllerSetup;

private:

    SCREEN_UI::TabHolder *m_TabHolder = nullptr;
    SCREEN_UI::Choice* m_BackButton;
    DirectoryBrowser *m_SearchDirBrowser;
    int m_LastTab;
    
    SpriteSheet* m_SpritesheetMarley;
    SpriteSheet m_SpritesheetTab;
    SpriteSheet m_SpritesheetBack;

    bool m_InputVSyncDolphin;
    int  m_InputResDolphin;
    int  m_GlobalVolume;
    bool m_GlobalVolumeEnabled;

};

