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
#include "marley/UI/browser/directoryBrowser.h"
#include "context.h"
#include "drawBuffer.h"
#include "i18n.h"
#include "root.h"

namespace MarleyApp
{

    DirectoryBrowser::DirectoryBrowser (
        std::string path, 
        DirectoryBrowserFlags browseFlags,
        SCREEN_ScreenManager *screenManager,
        std::string lastText,
        SpriteSheet* spritesheet,
        SCREEN_UI::LayoutParams *layoutParams)
            : LinearLayout(SCREEN_UI::ORIENT_VERTICAL, layoutParams), 
              path_(path),
              screenManager_(screenManager), 
              browseFlags_(browseFlags), 
              lastText_(lastText)
    {
        using namespace SCREEN_UI;
        //if (showTooltipSettingsScreen != "")
        //{
        //    SCREEN_UI::EventParams e{};
        //    e.v = this;
        //    settingsInfo_->Show(showTooltipSettingsScreen, e.v);
        //    showTooltipSettingsScreen = "";
        //}

        m_SpritesheetMarley = spritesheet;
        m_SpritesheetHome.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_HOME_R), 4 /* frames */);

        Refresh();
    }

    void DirectoryBrowser::SetPath(const std::string path)
    {
        path_.SetPath(path);
        Refresh();
    }

    std::string DirectoryBrowser::GetPath()
    {
        std::string str = path_.GetPath();
        return str;
    }

    void DirectoryBrowser::Touch(const SCREEN_TouchInput &input)
    {
        for (auto iter = views_.begin(); iter != views_.end(); ++iter)
        {
            (*iter)->Touch(input);
        }

        m_DirectoryListing->Touch(input);
    }

    SCREEN_UI::EventReturn DirectoryBrowser::HomeClick(SCREEN_UI::EventParams &e)
    {
        SetPath(Engine::m_Engine->GetHomeDirectory());
        if (GetDefaultFocusView())
        {
            SCREEN_UI::SetFocusedView(GetDefaultFocusView());
        }

        return SCREEN_UI::EVENT_DONE;
    }

    void DirectoryBrowser::Update()
    {
        LinearLayout::Update();
        if (listingPending_ && path_.IsListingReady())
        {
            Refresh();
        }
    }

    void DirectoryBrowser::Draw(SCREEN_UIContext &dc)
    {
        using namespace SCREEN_UI;

        for (View *view : views_)
        {
            if (view->GetVisibility() == V_VISIBLE)
            {
                if (dc.GetScissorBounds().Intersects(dc.TransformBounds(view->GetBounds())))
                {
                    view->Draw(dc);
                }
            }
        }
    }

    void DirectoryBrowser::Refresh()
    {
        using namespace SCREEN_UI;

        SetSpacing(0.0f);

        float availableWidth = Engine::m_Engine->GetContextWidth();
        float tabMarginLeftRight = 80.0f;
        float marginLeftRight = 128.0f;
        float iconWidth = 128.0f;
        float iconHeight = 128.0f;
        float iconSpacer = 10.0f;
        float gridButtonWidth = 350.0f;
        int gridButtonsPerLine = 4;
        float gridButtonSpacing = 5.0f;
        float scrollBarWidth = 20.0f;
        float verticalSpacer = 16.0f;
        float fileBrowserWidth = gridButtonsPerLine * gridButtonWidth + (gridButtonsPerLine-1) * gridButtonSpacing + scrollBarWidth;
        float fileBrowserHeight = 175.0f;
        float fileBrowserIndent = (availableWidth - fileBrowserWidth) / 2 - tabMarginLeftRight;

        // Reset content
        Clear();

        Add(new Spacer(10.0f));
        auto mm = GetI18NCategory("MainMenu");

        LinearLayout *topBar = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(FILL_PARENT, WRAP_CONTENT));

        Sprite* icon; 
        Sprite* icon_active;
        Sprite* icon_depressed;

        // home button
        Choice* m_HomeButton;
        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            icon = m_SpritesheetHome.GetSprite(BUTTON_4_STATES_NOT_FOCUSED); 
            icon_active = m_SpritesheetHome.GetSprite(BUTTON_4_STATES_FOCUSED); 
            icon_depressed = m_SpritesheetHome.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED); 
            m_HomeButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconHeight),true);
        }
        else
        {
            icon = m_SpritesheetMarley->GetSprite(I_HOME);
            m_HomeButton = new Choice(icon, new LayoutParams(iconWidth, iconHeight));
        }
        m_HomeButton->OnClick.Handle(this, &DirectoryBrowser::HomeClick);
        m_HomeButton->OnHighlight.Add([=](EventParams &e)
        {
            //if (!toolTipsShown[SETTINGS_HOME])
            //{
            //    toolTipsShown[SETTINGS_HOME] = true;
            //    settingsInfo_->Show(mm->T("Home", "Jump in file browser to home directory"), e.v);
            //}
            return SCREEN_UI::EVENT_CONTINUE;
        });
        topBar->Add(m_HomeButton);
        topBar->Add(new Spacer(34.0f));

        // display working directory
        TextView* workingDirectory;
        workingDirectory = new TextView(path_.GetFriendlyPath().c_str(), ALIGN_VCENTER | FLAG_WRAP_TEXT, false, new LinearLayoutParams(WRAP_CONTENT, 64.0f, 1.0f, G_VCENTER));
        topBar->Add(workingDirectory);

        Add(topBar);

        // info text
        //TextView* infoText1;
        //TextView* infoText2;
        //infoText1 = new TextView("To add a search path, highlight a folder and use the start button or space. ", 
        //             ALIGN_VCENTER | FLAG_WRAP_TEXT, false, new LinearLayoutParams(FILL_PARENT, f32));
        //infoText2 = new TextView("To remove a search path, scroll all the way down.", 
        //             ALIGN_VCENTER | FLAG_WRAP_TEXT, false, new LinearLayoutParams(FILL_PARENT, f32));
        //
        //Add(infoText1);
        //Add(infoText2);

        Add(new Spacer(verticalSpacer));

        LinearLayout *horizontalLayoutIndent = new LinearLayout(ORIENT_HORIZONTAL, new LayoutParams(FILL_PARENT, FILL_PARENT));
        ViewGroup* folderDisplayScroll = new ScrollView(ORIENT_VERTICAL, new LinearLayoutParams(fileBrowserWidth, fileBrowserHeight, 1.0f),true);
        horizontalLayoutIndent->Add(new Spacer(fileBrowserIndent));
        horizontalLayoutIndent->Add(folderDisplayScroll);
        Add(horizontalLayoutIndent);

        uint buttonTextMaxLength;

        m_DirectoryListing = new SCREEN_UI::GridLayout(SCREEN_UI::GridLayoutSettings(350.0f, 85.0f), new LinearLayoutParams(FILL_PARENT, WRAP_CONTENT));
        folderDisplayScroll->Add(m_DirectoryListing);
        buttonTextMaxLength = 15;

        // Show folders in the current directory
        m_UpButton = nullptr;
        m_DirButtons.clear();

        listingPending_ = !path_.IsListingReady();

        std::vector<std::string> filenames;
        if (!listingPending_) 
        {
            std::vector<File::FileInfo> fileInfo;
            path_.GetListing(fileInfo, "");
            for (size_t i = 0; i < fileInfo.size(); i++)
            {
                std::string str=fileInfo[i].name;

                if (fileInfo[i].isDirectory)
                {
                    if (browseFlags_ & DirectoryBrowserFlags::NAVIGATE)
                    {
                        m_DirButtons.push_back(new DirectoryBrowserButton(
                                                fileInfo[i].fullName.c_str(),
                                                fileInfo[i].name,
                                                m_SpritesheetMarley,
                                                buttonTextMaxLength, 
                                                new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::FILL_PARENT)));
                    }
                } 
            }
        }

        if (browseFlags_ & DirectoryBrowserFlags::NAVIGATE)
        {
            m_UpButton = new DirectoryBrowserButton("..", m_SpritesheetMarley, 2, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::FILL_PARENT));
            m_UpButton->OnClick.Handle(this, &DirectoryBrowser::NavigateClick);
            m_DirectoryListing->Add(m_UpButton);

        }

        if (listingPending_)
        {
            m_DirectoryListing->Add(new SCREEN_UI::TextView(mm->T("Loading..."), ALIGN_CENTER, false, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::FILL_PARENT)));
        }

        for (size_t i = 0; i < m_DirButtons.size(); i++)
        {
            std::string str = "DirectoryBrowser (" + std::to_string(i) + ") " + m_DirButtons[i]->GetPath();
            m_DirButtons[i]->SetTag(str);
            m_DirectoryListing->Add(m_DirButtons[i])->OnClick.Handle(this, &DirectoryBrowser::NavigateClick);
        }
    }

    const std::string DirectoryBrowser::GetBaseName(const std::string &path)
    {

        static const std::string sepChars = "/";

        auto trailing = path.find_last_not_of(sepChars);
        if (trailing != path.npos)
        {
            size_t start = path.find_last_of(sepChars, trailing);
            if (start != path.npos)
            {
                return path.substr(start + 1, trailing - start);
            }
            return path.substr(0, trailing);
        }

        size_t start = path.find_last_of(sepChars);
        if (start != path.npos)
        {
            return path.substr(start + 1);
        }
        return path;
    }

    SCREEN_UI::EventReturn DirectoryBrowser::NavigateClick(SCREEN_UI::EventParams &e)
    {
        DirectoryBrowserButton *button = static_cast<DirectoryBrowserButton *>(e.v);
        std::string text = button->GetPath();
        if (button->PathAbsolute())
        {
            path_.SetPath(text);
        }
        else
        {
            path_.Navigate(text);
        }
        Refresh();
        if (GetDefaultFocusView())
        {
            SCREEN_UI::SetFocusedView(GetDefaultFocusView());
        }
        else
        {
            SCREEN_UI::SetFocusedView(m_DirButtons[0]);
        }
        return SCREEN_UI::EVENT_DONE;
    }

    SCREEN_UI::EventReturn DirectoryBrowser::OnRecentClear(SCREEN_UI::EventParams &e)
    {
        screenManager_->RecreateAllViews();
        return SCREEN_UI::EVENT_DONE;
    }
}
