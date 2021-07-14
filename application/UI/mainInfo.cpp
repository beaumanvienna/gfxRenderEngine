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
#include "mainInfo.h"
#include "viewGroup.h"

MainInfoMessage::MainInfoMessage(int align, SCREEN_UI::AnchorLayoutParams *lp)
    : SCREEN_UI::LinearLayout(SCREEN_UI::ORIENT_HORIZONTAL, lp)
{
    using namespace SCREEN_UI;
    SetSpacing(0.0f);
    Add(new SCREEN_UI::Spacer(10.0f));
    text_ = Add(new SCREEN_UI::TextView("", align, false, new LinearLayoutParams(1.0, Margins(0, 10))));
    Add(new SCREEN_UI::Spacer(10.0f));
    
    m_ContextWidth  = Engine::m_Engine->GetContextWidth();
    m_ContextHeight = Engine::m_Engine->GetContextHeight();
    m_HalfContextWidth  = m_ContextWidth  * 0.5f;
    m_HalfContextHeight = m_ContextHeight * 0.5f;
    
}

void MainInfoMessage::Show(const std::string &text, SCREEN_UI::View *refView)
{
    if (refView)
    {
        Bounds b = refView->GetBounds();
        const SCREEN_UI::AnchorLayoutParams *lp = GetLayoutParams()->As<SCREEN_UI::AnchorLayoutParams>();
        if (b.y >= cutOffY_)
        {
            ReplaceLayoutParams(new SCREEN_UI::AnchorLayoutParams(lp->width, lp->height, lp->left, 80.0f, lp->right, lp->bottom, lp->center));
        }
        else
        {
            ReplaceLayoutParams(new SCREEN_UI::AnchorLayoutParams(lp->width, lp->height, lp->left, m_ContextHeight - 80.0f - 40.0f, lp->right, lp->bottom, lp->center));
        }
    }
    text_->SetText(text);
    timeShown_ = Engine::m_Engine->GetTime();
}

void MainInfoMessage::Draw(SCREEN_UIContext &dc)
{
    static const double FADE_TIME = 1.0;
    static const float MAX_ALPHA = 0.9f;

    double timeToShow = std::max(1.5, text_->GetText().size() * 0.05);

    double sinceShow = Engine::m_Engine->GetTime() - timeShown_;
    float alpha = MAX_ALPHA;
    if (timeShown_ == 0.0 || sinceShow > timeToShow + FADE_TIME)
    {
        alpha = 0.0f;
    }
    else if (sinceShow > timeToShow)
    {
        alpha = MAX_ALPHA - MAX_ALPHA * (float)((sinceShow - timeToShow) / FADE_TIME);
    }

    if (alpha >= 0.1f)
    {
        SCREEN_UI::Style style = dc.theme->popupTitle;
        style.background.color = colorAlpha(style.background.color, alpha - 0.1f);
        dc.FillRect(style.background, bounds_);
    }

    text_->SetTextColor(whiteAlpha(alpha));
    text_->SetShadow(false);
    ViewGroup::Draw(dc);
}
