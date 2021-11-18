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
#include "marley/marley.h"
#include "marley/UI/browser/directoryBrowserButton.h"
#include "context.h"
#include "screen.h"
#include "drawBuffer.h"

namespace MarleyApp
{

    void DirectoryBrowserButton::Draw(SCREEN_UIContext &dc)
    {
        using namespace SCREEN_UI;
        Style style = dc.theme->buttonStyle;

        if (HasFocus()) style = dc.theme->buttonFocusedStyle;
        if (down_) style = dc.theme->buttonDownStyle;
        if (!IsEnabled()) style = dc.theme->buttonDisabledStyle;

        dc.FillRect(style.background, bounds_);

        const std::string text = GetText();

        bool isRegularFolder = true;
        Sprite* image;
        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            image = m_SpritesheetMarley->GetSprite(I_FOLDER_R);
        } 
        else
        {
            image = m_SpritesheetMarley->GetSprite(I_FOLDER);
        }
        if (text == "..")
        {
            isRegularFolder = false;
            if (CoreSettings::m_UITheme == THEME_RETRO)
            {
                image = m_SpritesheetMarley->GetSprite(I_UP_DIRECTORY_R);
            }
            else
            {
                image = m_SpritesheetMarley->GetSprite(I_UP_DIRECTORY);
            }
        }

        dc.SetFontStyle(dc.theme->uiFontSmall);

        float tw, th;
        dc.MeasureText(dc.GetFontStyle(), 1.0f, 1.0f, text.c_str(), &tw, &th, 0);

        bool compact = true;

        dc.SetFontScale(1.0f, 1.0f);
        if (compact)
        {
            // No icon, except "up"
            dc.PushScissor(bounds_);
            if (isRegularFolder)
            {
                if (CoreSettings::m_UITheme == THEME_RETRO)
                {
                    dc.DrawText(text.c_str(), bounds_.x + 7, bounds_.centerY()+2, RETRO_COLOR_FONT_BACKGROUND, ALIGN_VCENTER);
                }
                dc.DrawText(text.c_str(), bounds_.x + 5, bounds_.centerY(), style.fgColor, ALIGN_VCENTER);
            }
            else
            {
                dc.Draw()->DrawImage(image, bounds_.centerX(), bounds_.centerY(), 1.0f, 0xFFFFFFFF, ALIGN_CENTER);
            }
            dc.PopScissor();
        } 
        else
        {
            bool scissor = false;
            if (tw + 150 > bounds_.w)
            {
                dc.PushScissor(bounds_);
                scissor = true;
            }
            dc.Draw()->DrawImage(image, bounds_.x + 72, bounds_.centerY(), 0.88f, 0xFFFFFFFF, ALIGN_CENTER);
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
    }

    bool DirectoryBrowserButton::Key(const SCREEN_KeyInput &key)
    {
        if (key.flags & KEY_DOWN)
        {
            if (HasFocus() && ((key.keyCode == Controller::BUTTON_START) || (key.keyCode == ENGINE_KEY_SPACE)))
            {
                if (m_Path != "..")
                {
                    Marley::m_BiosFiles.SetSearchPath(m_Path);
                    Marley::m_BiosFiles.InstallAllFiles();
                    UI::m_ScreenManager->RecreateAllViews();
                }
            }
        } 

        return Clickable::Key(key);
    }
}

