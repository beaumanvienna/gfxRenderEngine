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

#include "common.h"
#include "core.h"
#include "marley/UI/browser/ROMbutton.h"
#include "context.h"
#include "screen.h"
#include "drawBuffer.h"

namespace MarleyApp
{

    void ROMButton::Draw(SCREEN_UIContext &dc)
    {
        using namespace SCREEN_UI;
        Style style = dc.theme->buttonStyle;

        if (HasFocus()) style = dc.theme->buttonFocusedStyle;
        if (down_) style = dc.theme->buttonDownStyle;
        if (!IsEnabled()) style = dc.theme->buttonDisabledStyle;

        dc.FillRect(style.background, bounds_);

        int startChar = m_GamePath.find_last_of("/") + 1;  //show only file name
        int endChar = m_GamePath.find_last_of("."); // remove extension
        const std::string text = m_GamePath.substr(startChar,endChar-startChar);

        Sprite* image = SCREEN_ScreenManager::m_SpritesheetUI->GetSprite(I_BARREL);

        float tw, th;
        dc.MeasureText(dc.GetFontStyle(), 1.0f, 1.0f, text.c_str(), &tw, &th, 0);


        bool scissor = false;
        if (tw + 150 > bounds_.w)
        {
            dc.PushScissor(bounds_);
            scissor = true;
        }
        dc.Draw()->DrawImage(image, bounds_.x + 72, bounds_.centerY(), 1.0f, 0xFFFFFFFF, ALIGN_CENTER);
        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            dc.DrawText(text.c_str(), bounds_.x + 152, bounds_.centerY()+2, RETRO_COLOR_FONT_BACKGROUND, ALIGN_VCENTER);
        }
        dc.DrawText(text.c_str(), bounds_.x + 150, bounds_.centerY(), style.fgColor, ALIGN_VCENTER);

        if (scissor)
        {
            dc.PopScissor();
        }
    }

    void ROMButton::GetContentDimensions(const SCREEN_UIContext &dc, float &w, float &h) const
    {
        w = 500.0f;
        h = 50.0f;
    }

    void ROMButton::SetHoldEnabled(bool hold)
    {
        m_HoldEnabled = hold;
    }

    bool ROMButton::Touch(const SCREEN_TouchInput &input)
    {
        bool clicked = SCREEN_UI::Clickable::Touch(input);
        m_Hovering = bounds_.Contains(input.x, input.y);
        if (m_Hovering && (input.flags & TOUCH_DOWN))
        {
            m_HoldStart = Engine::m_Engine->GetTime();
        }
        else if (input.flags & TOUCH_UP)
        {
            m_HoldStart = 0;
        }
        return clicked;
    }

    bool ROMButton::Key(const SCREEN_KeyInput &key)
    {
        return Clickable::Key(key);
    }

    void ROMButton::Update() 
    {
        // Hold button for 1.5 seconds to launch the game options
        if (m_HoldEnabled && m_HoldStart != 0.0 && m_HoldStart < Engine::m_Engine->GetTime() - 1500000) 
        {
            TriggerOnHoldClick();
        }
    }

    void ROMButton::FocusChanged(int focusFlags)
    {
        SCREEN_UI::Clickable::FocusChanged(focusFlags);
        TriggerOnHighlight(focusFlags);
    }

    void ROMButton::TriggerOnHoldClick()
    {
        m_HoldStart = 0.0;
        SCREEN_UI::EventParams e{};
        e.v = this;
        e.s = m_GamePath;
        down_ = false;
        OnHoldClick.Trigger(e);
    }

    void ROMButton::TriggerOnHighlight(int focusFlags)
    {
        SCREEN_UI::EventParams e{};
        e.v = this;
        e.s = m_GamePath;
        e.a = focusFlags;
        OnHighlight.Trigger(e);
    }
}
