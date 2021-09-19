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
#include "viewGroup.h"
#include "pathBrowser.h"
#include "marley/UI/browser/dirButton.h"
#include "marley/emulation/emulationEvent.h"

namespace MarleyApp
{

    class ROMBrowser : public SCREEN_UI::LinearLayout
    {
    public:
        ROMBrowser(std::string path, SCREEN_UI::TextView* gamesPathView, SCREEN_UI::LayoutParams *layoutParams = nullptr);
        ~ROMBrowser();

        virtual void Touch(const SCREEN_TouchInput &input) override;

        SCREEN_UI::Event OnHoldChoice;
        SCREEN_UI::Event OnROMClick;
        SCREEN_UI::Event OnNavigateClick;

        void SetPath(const std::string &path);
        std::string GetPath();
        void Draw(SCREEN_UIContext &dc) override;
        void Update() override;
        View* GetDefaultFocusView() const { return m_UpButton; }
        
        void SetEventCallback(const EventCallbackFunction& callback);

    protected:

        void Refresh();

    private:

        const std::string GetBaseName(const std::string &path);
        
        EventCallbackFunction m_EventCallback;

        SCREEN_UI::EventReturn ROMButtonClick(SCREEN_UI::EventParams &e);
        SCREEN_UI::EventReturn NavigateClick(SCREEN_UI::EventParams &e);

        SCREEN_UI::ViewGroup* m_GameList = nullptr;
        SCREEN_UI::TextView* m_GamesPathView;
        DirButtonMain* m_UpButton;
        SCREEN_PathBrowser m_Path;
        bool m_ListingPending = false;
        std::string m_LastGamePath;
        std::vector<DirButtonMain*> m_DirButtons;
    };
}
