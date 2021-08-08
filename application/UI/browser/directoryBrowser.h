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

#include <vector>
#include <iostream>

#include "engine.h"
#include "screen.h"
#include "viewGroup.h"
#include "pathBrowser.h"
#include "browser/dirButton.h"
#include "browser/directoryBrowserButton.h"
#include "spritesheet.h"

enum class DirectoryBrowserFlags
{
    NONE            = 0,
    NAVIGATE        = 1,
    ARCHIVES        = 2,
    PIN             = 4,
    HOMEBREW_STORE  = 8,
    STANDARD        = 1 | 2 | 4,
};
ENUM_CLASS_BITOPS(DirectoryBrowserFlags)

class DirectoryBrowser : public SCREEN_UI::LinearLayout
{
public:
    DirectoryBrowser
    (
        std::string path,
        DirectoryBrowserFlags browseFlags,
        SCREEN_ScreenManager *screenManager,
        std::string lastText,
        SpriteSheet* spritesheet,
        SCREEN_UI::LayoutParams *layoutParams = nullptr
    );
    ~DirectoryBrowser() {}
    
    virtual void Touch(const SCREEN_TouchInput &input) override;
    
    SCREEN_UI::Event OnChoice;
    SCREEN_UI::Event OnHoldChoice;
    SCREEN_UI::Event OnHighlight;

    void SetPath(const std::string path);
    std::string GetPath();
    void Draw(SCREEN_UIContext &dc) override;
    void Update() override;
    View* GetDefaultFocusView() const { return m_UpButton; }

protected:

    void Refresh();

private:

    bool IsCurrentPathPinned();
    const std::vector<std::string> GetPinnedPaths();
    const std::string GetBaseName(const std::string &path);

    SCREEN_UI::EventReturn NavigateClick(SCREEN_UI::EventParams &e);
    SCREEN_UI::EventReturn LayoutChange(SCREEN_UI::EventParams &e);
    SCREEN_UI::EventReturn HomeClick(SCREEN_UI::EventParams &e);
    SCREEN_UI::EventReturn OnRecentClear(SCREEN_UI::EventParams &e);

    SpriteSheet* m_SpritesheetMarley;
    SpriteSheet m_SpritesheetHome;

    SCREEN_UI::Choice* m_HomeButton;
    DirectoryBrowserButton* m_UpButton;

    SCREEN_UI::ViewGroup *m_DirectoryListing = nullptr;
    SCREEN_PathBrowser path_;
    DirectoryBrowserFlags browseFlags_;
    std::string lastText_;
    bool listingPending_ = false;
    SCREEN_ScreenManager *screenManager_;
    std::vector<DirectoryBrowserButton*> m_DirButtons;
};
