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

namespace MarleyApp
{
    class PauseDialog : public SCREEN_PopupScreen
    {

    public:
        PauseDialog(std::string label, SpriteSheet* spritesheetMarley )
            : SCREEN_PopupScreen(label), m_ChangeTitle(false),
              m_SpritesheetMarley(spritesheetMarley)  {}

        void CreatePopupContents(SCREEN_UI::ViewGroup *parent) override;
        bool key(const SCREEN_KeyInput &touch) override;

    private:

        SCREEN_UI::EventReturn ExitEmulation(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn Return(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn Save(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn Load(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn Highlight(SCREEN_UI::EventParams &e);
        void ExitPauseDialog();
        
        SpriteSheet* m_SpritesheetMarley;
        
        Sprite* m_Icon;
        Sprite* m_Icon_active;
        Sprite* m_Icon_depressed;

        SCREEN_UI::Choice* m_BackButton;
        SCREEN_UI::Choice* m_SaveButton;
        SCREEN_UI::Choice* m_LoadButton;
        SCREEN_UI::Choice* m_OffButton;

        SpriteSheet m_SpritesheetBack;
        SpriteSheet m_SpritesheetSave;
        SpriteSheet m_SpritesheetLoad;
        SpriteSheet m_SpritesheetOff;
        
        bool m_ChangeTitle;

    };
}
