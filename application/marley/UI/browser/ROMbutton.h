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

#include "view.h"

namespace MarleyApp
{

    class ROMButton : public SCREEN_UI::Clickable
    {
    public:

        ROMButton(const std::string &gamePath, SCREEN_UI::LayoutParams *layoutParams = 0)
            : SCREEN_UI::Clickable(layoutParams), m_GamePath(gamePath) {}

        void Update() override;
        void Draw(SCREEN_UIContext &dc) override;
        void Touch(const SCREEN_TouchInput &input) override;
        bool Key(const SCREEN_KeyInput &key) override;
        void GetContentDimensions(const SCREEN_UIContext &dc, float &w, float &h) const override;
        void FocusChanged(int focusFlags) override;
        
        const std::string& GetPath() const { return m_GamePath; }
        void SetHoldEnabled(bool hold);

        SCREEN_UI::Event OnHoldClick;
        SCREEN_UI::Event OnHighlight;

    private:

        void TriggerOnHoldClick();
        void TriggerOnHighlight(int focusFlags);

        std::string m_GamePath;

        double m_HoldStart = 0.0;
        bool m_HoldEnabled = true;
        bool m_Hovering = false;
    };
}
