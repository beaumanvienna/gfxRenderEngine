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
#include "viewGroup.h"
#include "pathBrowser.h"
#include "browser/dirButton.h"

class ROMBrowser : public SCREEN_UI::LinearLayout
{
public:
    ROMBrowser(std::string path, SCREEN_UI::TextView* gamesPathView, SCREEN_UI::LayoutParams *layoutParams = nullptr);
    ~ROMBrowser();
    SCREEN_UI::Event OnChoice;
    SCREEN_UI::Event OnHoldChoice;
    SCREEN_UI::Event OnHighlight;

    void SetPath(const std::string &path);
    std::string GetPath();
    void Draw(SCREEN_UIContext &dc) override;
    void Update() override;
    View* GetDefaultFocusView() const { return m_UPButton; }

protected:

    void Refresh();

private:

    const std::string GetBaseName(const std::string &path);

    SCREEN_UI::EventReturn GameButtonClick(SCREEN_UI::EventParams &e);
    SCREEN_UI::EventReturn NavigateClick(SCREEN_UI::EventParams &e);

    SCREEN_UI::ViewGroup *gameList_ = nullptr;
    SCREEN_UI::TextView* m_GamesPathView;
    DirButtonMain* m_UPButton;
    SCREEN_PathBrowser path_;
    std::string focusGamePath_;
    bool listingPending_ = false;
    float lastScale_ = 1.0f;
    std::string m_LastGamePath;
};

