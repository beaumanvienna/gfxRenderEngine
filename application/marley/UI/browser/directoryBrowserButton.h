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
#include "keyCodes.h"
#include "controller.h"
#include "marley/emulation/bios.h"

namespace MarleyApp
{

    class DirectoryBrowserButton : public SCREEN_UI::Button
    {
    public:
        DirectoryBrowserButton(const std::string &path, SpriteSheet* spritesheetMarley, uint maxTextLength, SCREEN_UI::LayoutParams *layoutParams)
            : SCREEN_UI::Button(path, maxTextLength, layoutParams), m_Path(path), m_AbsolutePath(false), m_SpritesheetMarley(spritesheetMarley) {}
        DirectoryBrowserButton(const std::string &path, const std::string &text, SpriteSheet* spritesheetMarley, uint maxTextLength, SCREEN_UI::LayoutParams* layoutParams = nullptr)
            : SCREEN_UI::Button(text, maxTextLength, layoutParams), m_Path(path), m_AbsolutePath(true), m_SpritesheetMarley(spritesheetMarley) {}

        virtual void Draw(SCREEN_UIContext &dc);

        const std::string GetPath() const { return m_Path; }

        bool PathAbsolute() const { return m_AbsolutePath; }

        bool Key(const SCREEN_KeyInput &key) override;

    private:
        SpriteSheet* m_SpritesheetMarley;
        std::string m_Path;
        bool m_AbsolutePath;
    };
}
