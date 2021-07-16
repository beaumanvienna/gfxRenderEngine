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
#include "browser/ROMbrowser.h"
#include "browser/ROMbutton.h"
#include "browser/dirButton.h"
#include "context.h"
#include "drawBuffer.h"
#include "i18n.h"

ROMBrowser::ROMBrowser(std::string path, SCREEN_UI::TextView* gamesPathView, SCREEN_UI::LayoutParams *layoutParams)
    : LinearLayout(SCREEN_UI::ORIENT_VERTICAL, layoutParams), path_(path), m_GamesPathView(gamesPathView)
{
    using namespace SCREEN_UI;
    Refresh();
}

ROMBrowser::~ROMBrowser()
{}

void ROMBrowser::SetPath(const std::string &path)
{
    path_.SetPath(path);
    Refresh();
}

std::string ROMBrowser::GetPath()
{
    std::string str = path_.GetPath();
    return str;
}

void ROMBrowser::Update()
{
    LinearLayout::Update();
    if (listingPending_ && path_.IsListingReady())
    {
        Refresh();
    }
}

void ROMBrowser::Draw(SCREEN_UIContext &dc)
{
    using namespace SCREEN_UI;

    if (lastScale_ != 1.0f)
    {
        Refresh();
    }

    if (hasDropShadow_)
    {
        dc.FillRect(SCREEN_UI::Drawable(0x60000000), dc.GetBounds().Expand(dropShadowExpand_));
        float dropsize = 30.0f;
        dc.Draw()->DrawImage4Grid(dc.theme->dropShadow4Grid,
            bounds_.x - dropsize, bounds_.y,
            bounds_.x2() + dropsize, bounds_.y2()+dropsize*1.5f, 0xDF000000, 3.0f);
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
            // Check if bounds are in current scissor rectangle.
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

void ROMBrowser::Refresh()
{
    using namespace SCREEN_UI;
    
    lastScale_ = 1.0f;

    // Reset content
    Clear();

    Add(new Spacer(1.0f));
    auto mm = GetI18NCategory("MainMenu");

    SCREEN_UI::LinearLayout *linearLayout = new SCREEN_UI::LinearLayout(SCREEN_UI::ORIENT_VERTICAL, new LinearLayoutParams(FILL_PARENT, WRAP_CONTENT));
    linearLayout->SetSpacing(4.0f);
    gameList_ = linearLayout;

    Add(gameList_);

    // Show games in the current directory
    std::vector<ROMButton *> gameButtons;

    // Show folders in the current directory
    std::vector<DirButtonMain *> dirButtons;

    listingPending_ = !path_.IsListingReady();

    std::vector<std::string> filenames;
    if (!listingPending_)
    {
        m_LastGamePath = path_.GetPath();
        m_GamesPathView->SetText(path_.GetFriendlyPath().c_str());

        std::list<std::string> tmpList;
        std::list<std::string> toBeRemoved;
        std::string strList;
        std::list<std::string>::iterator iteratorTmpList;

        //stopSearching=false;
        //findAllFiles(m_LastGamePath.c_str(),&tmpList,&toBeRemoved,false);
        //stripList(&tmpList,&toBeRemoved); // strip cue file entries
        //finalizeList(&tmpList);

        iteratorTmpList = tmpList.begin();
        for (int i=0;i<tmpList.size();i++)
        {
            strList = *iteratorTmpList;
            iteratorTmpList++;
            gameButtons.push_back(new ROMButton(strList, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, 50.0f)));
        }

        std::vector<File::FileInfo> fileInfo;
        path_.GetListing(fileInfo);
        for (size_t i = 0; i < fileInfo.size(); i++)
        {
            if (fileInfo[i].isDirectory)
            {
                dirButtons.push_back(new DirButtonMain(fileInfo[i].fullName.c_str(), fileInfo[i].name, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, 50.0f)));
            }
        }
    }

    if (m_LastGamePath != "/")
    {
        DirButtonMain* UP_button = new DirButtonMain("..", new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, 50.0f));
        UP_button->OnClick.Handle(this, &ROMBrowser::NavigateClick);
        gameList_->Add(UP_button);
    }

    if (listingPending_)
    {
        gameList_->Add(new SCREEN_UI::TextView(mm->T("Loading..."), ALIGN_CENTER, false, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::FILL_PARENT)));
    }

    for (size_t i = 0; i < gameButtons.size(); i++)
    {
        gameList_->Add(gameButtons[i])->OnClick.Handle(this, &ROMBrowser::GameButtonClick);
    }

    for (size_t i = 0; i < dirButtons.size(); i++)
    {
        std::string str = dirButtons[i]->GetPath();
        gameList_->Add(dirButtons[i])->OnClick.Handle(this, &ROMBrowser::NavigateClick);
    }
}

const std::string ROMBrowser::GetBaseName(const std::string &path)
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

SCREEN_UI::EventReturn ROMBrowser::NavigateClick(SCREEN_UI::EventParams &e)
{
    DirButtonMain *button = static_cast<DirButtonMain *>(e.v);
    std::string text = button->GetPath();
    if (button->PathAbsolute()) 
    {
        path_.SetPath(text);
    }
    else
    { // cd ..
        path_.Navigate(text);
    }
    Refresh();
    return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn ROMBrowser::GameButtonClick(SCREEN_UI::EventParams &e)
{
    ROMButton *button = static_cast<ROMButton *>(e.v);
    std::string text = button->GetPath();
    
//    launch_request_from_screen_manager = true;
//    game_screen_manager = text;

    SCREEN_System_SendMessage("finish", "");

    return SCREEN_UI::EVENT_DONE;
}
