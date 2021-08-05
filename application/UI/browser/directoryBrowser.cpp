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
#include "browser/directoryBrowser.h"
#include "browser/directoryBrowserButton.h"
#include "context.h"
#include "drawBuffer.h"
#include "i18n.h"
#include "root.h"

DirectoryBrowser::DirectoryBrowser (
    std::string path, 
    DirectoryBrowserFlags browseFlags,
    bool *gridStyle,
    SCREEN_ScreenManager *screenManager,
    std::string lastText,
    SpriteSheet* spritesheet,
    SCREEN_UI::LayoutParams *layoutParams)
        : LinearLayout(SCREEN_UI::ORIENT_VERTICAL, layoutParams), 
          path_(path), 
          m_GridStyle(gridStyle), 
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
    m_SpritesheetGrid.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_GRID_R), 4 /* frames */);
    m_SpritesheetLines.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_LINES_R), 4 /* frames */);

    Refresh();
}

void DirectoryBrowser::FocusGame(const std::string &gamePath)
{
    focusGamePath_ = gamePath;
    Refresh();
    focusGamePath_.clear();
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

SCREEN_UI::EventReturn DirectoryBrowser::LayoutChange(SCREEN_UI::EventParams &e)
{
    *m_GridStyle = e.a == 0 ? true : false;
    Refresh();
    return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn DirectoryBrowser::HomeClick(SCREEN_UI::EventParams &e)
{
    SetPath(Engine::m_Engine->GetHomeDirectory());

    return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn DirectoryBrowser::GridClick(SCREEN_UI::EventParams &e)
{
    *m_GridStyle  = true;
    Refresh();
    return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn DirectoryBrowser::LinesClick(SCREEN_UI::EventParams &e)
{
    *m_GridStyle  = false;
    Refresh();
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

    if (lastScale_ != 1.0f || lastLayoutWasGrid_ != *m_GridStyle)
    {
        Refresh();
    }

    if (hasDropShadow_)
    {
        dc.FillRect(SCREEN_UI::Drawable(0x60000000), dc.GetBounds().Expand(dropShadowExpand_));
        float dropsize = 30.0f;
        dc.Draw()->DrawImage4Grid
        (
            dc.theme->dropShadow4Grid, bounds_.x - dropsize, bounds_.y,
            bounds_.x2() + dropsize, bounds_.y2()+dropsize*1.5f, 0xDF000000, 3.0f
        );
    }

    if (clip_)
    {
        dc.PushScissor(bounds_);
    }

    dc.FillRect(bg_, bounds_);
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
    if (clip_)
    {
        dc.PopScissor();
    }
}

void DirectoryBrowser::Refresh()
{
    using namespace SCREEN_UI;

    lastScale_ = 1.0f;
    lastLayoutWasGrid_ = *m_GridStyle;
    
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
    float fileBrowserWidth = gridButtonsPerLine * gridButtonWidth + (gridButtonsPerLine-1) * gridButtonSpacing + scrollBarWidth;
    float fileBrowserHeight = 175.0f;
    float fileBrowserIndent = (availableWidth - fileBrowserWidth) / 2 - tabMarginLeftRight;

    // Reset content
    Clear();

    Add(new Spacer(10.0f));
    auto mm = GetI18NCategory("MainMenu");

    LinearLayout *topBar = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(FILL_PARENT, WRAP_CONTENT));

    // display working directory
    TextView* workingDirectory;
    workingDirectory = new TextView(path_.GetFriendlyPath().c_str(), ALIGN_VCENTER | FLAG_WRAP_TEXT, false, new LinearLayoutParams(Engine::m_Engine->GetContextWidth() - 580.0f, 64.0f, 1.0f, G_VCENTER));
    topBar->Add(workingDirectory);
    topBar->Add(new Spacer(50.0f));
    
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
    
    // grid button
    if (CoreSettings::m_UITheme == THEME_RETRO)
    {
        icon = m_SpritesheetGrid.GetSprite(BUTTON_4_STATES_NOT_FOCUSED); 
        icon_active = m_SpritesheetGrid.GetSprite(BUTTON_4_STATES_FOCUSED); 
        icon_depressed = m_SpritesheetGrid.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED); 
        m_GridButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconHeight),true);
    }
    else
    {
        icon = m_SpritesheetMarley->GetSprite(I_GRID);
        m_GridButton = new Choice(icon, new LayoutParams(iconWidth, iconHeight));
    }
    m_GridButton->OnClick.Handle(this, &DirectoryBrowser::GridClick);
    m_GridButton->OnHighlight.Add([=](EventParams &e) {
        //if (!toolTipsShown[SETTINGS_GRID])
        //{
        //    toolTipsShown[SETTINGS_GRID] = true;
        //    settingsInfo_->Show(mm->T("Grid", "Show file browser in a grid"), e.v);
        //}
        return SCREEN_UI::EVENT_CONTINUE;
    });
    topBar->Add(m_GridButton);
    
    // lines button
    if (CoreSettings::m_UITheme == THEME_RETRO)
    {
        icon = m_SpritesheetLines.GetSprite(BUTTON_4_STATES_NOT_FOCUSED); 
        icon_active = m_SpritesheetLines.GetSprite(BUTTON_4_STATES_FOCUSED); 
        icon_depressed = m_SpritesheetLines.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED); 
        m_LinesButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconHeight),true);
    }
    else
    {
        icon = m_SpritesheetMarley->GetSprite(I_LINES);
        m_LinesButton = new Choice(icon, new LayoutParams(iconWidth, iconHeight));
    }
    m_LinesButton->OnClick.Handle(this, &DirectoryBrowser::LinesClick);
    m_LinesButton->OnHighlight.Add([=](EventParams &e) 
    {
        //if (!toolTipsShown[SETTINGS_LINES])
        //{
        //    toolTipsShown[SETTINGS_LINES] = true;
        //    settingsInfo_->Show(mm->T("Lines", "Show file browser in lines"), e.v);
        //}
        return SCREEN_UI::EVENT_CONTINUE;
    });
    topBar->Add(m_LinesButton);
    
    Add(topBar);
    Add(new Spacer(5.0f));
    SetSpacing(0.0f);
    
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
    
    Add(new Spacer(80.0f));
    
    LinearLayout *horizontalLayoutIndent = new LinearLayout(ORIENT_HORIZONTAL, new LayoutParams(FILL_PARENT, FILL_PARENT));
    ViewGroup* folderDisplayScroll = new ScrollView(ORIENT_VERTICAL, new LinearLayoutParams(fileBrowserWidth, fileBrowserHeight, 1.0f),true);
    horizontalLayoutIndent->Add(new Spacer(fileBrowserIndent));
    horizontalLayoutIndent->Add(folderDisplayScroll);
    Add(horizontalLayoutIndent);
    
    uint buttonTextMaxLength;
    if (*m_GridStyle)
    {
        m_DirectoryListing = new SCREEN_UI::GridLayout(SCREEN_UI::GridLayoutSettings(350.0f, 85.0f), new LinearLayoutParams(FILL_PARENT, WRAP_CONTENT));
        folderDisplayScroll->Add(m_DirectoryListing);
        buttonTextMaxLength = 15;
    }
    else
    {
        SCREEN_UI::LinearLayout *directoryListingLines = new SCREEN_UI::LinearLayout(SCREEN_UI::ORIENT_VERTICAL, new LinearLayoutParams(FILL_PARENT, 60.0f, 0.0f, G_TOPLEFT));
        directoryListingLines->SetSpacing(4.0f);
        m_DirectoryListing = directoryListingLines;
        folderDisplayScroll->Add(m_DirectoryListing);
        buttonTextMaxLength = 40;
    }

    // Show folders in the current directory
    std::vector<DirectoryBrowserButton*> dirButtons;

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
                    dirButtons.push_back(new DirectoryBrowserButton(
                                            fileInfo[i].fullName.c_str(),
                                            fileInfo[i].name,
                                            *m_GridStyle,
                                            m_SpritesheetMarley,
                                            buttonTextMaxLength, 
                                            new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::FILL_PARENT)));
                }
            } 
        }
    }

    if (browseFlags_ & DirectoryBrowserFlags::NAVIGATE)
    {
        m_DirectoryListing->Add(new DirectoryBrowserButton("..", *m_GridStyle, m_SpritesheetMarley, 2, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::FILL_PARENT)))->
            OnClick.Handle(this, &DirectoryBrowser::NavigateClick);

    }

    if (listingPending_)
    {
        m_DirectoryListing->Add(new SCREEN_UI::TextView(mm->T("Loading..."), ALIGN_CENTER, false, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::FILL_PARENT)));
    }

    for (size_t i = 0; i < dirButtons.size(); i++)
    {
        std::string str = dirButtons[i]->GetPath();
        m_DirectoryListing->Add(dirButtons[i])->OnClick.Handle(this, &DirectoryBrowser::NavigateClick);
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
    return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn DirectoryBrowser::OnRecentClear(SCREEN_UI::EventParams &e)
{    
    screenManager_->RecreateAllViews();
    return SCREEN_UI::EVENT_DONE;
}
