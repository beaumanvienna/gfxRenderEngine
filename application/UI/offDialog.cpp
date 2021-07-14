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
#include "offDialog.h"
#include "i18n.h"
#include "viewGroup.h"

#define TRANSPARENT_BACKGROUND true
void OffDialog::CreatePopupContents(SCREEN_UI::ViewGroup *parent)
{
    using namespace SCREEN_UI;

    auto ma = GetI18NCategory("Main");

    LinearLayout *items = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(WRAP_CONTENT, WRAP_CONTENT));

    if (gTheme == THEME_RETRO)
    {
        if (m_offDiagEvent == OFFDIAG_QUIT)
        {
            items->Add(new Choice(ma->T("YES"), TRANSPARENT_BACKGROUND, new LayoutParams(200.0f, 64.0f)))->OnClick.Handle(this, &OffDialog::QuitMarley);
            items->Add(new Choice(ma->T("CANCEL"), TRANSPARENT_BACKGROUND, new LayoutParams(200.0f, 64.0f)))->OnClick.Handle<SCREEN_UIScreen>(this, &SCREEN_UIScreen::OnBack);
        }
        else
        {
            items->Add(new Choice(ma->T("YES"), TRANSPARENT_BACKGROUND, new LayoutParams(200.0f, 64.0f)))->OnClick.Handle(this, &OffDialog::SwitchOff);
            items->Add(new Choice(ma->T("CANCEL"), TRANSPARENT_BACKGROUND, new LayoutParams(200.0f, 64.0f)))->OnClick.Handle<SCREEN_UIScreen>(this, &SCREEN_UIScreen::OnBack);
        }
    } else
    {
        if (m_offDiagEvent == OFFDIAG_QUIT)
        {
            items->Add(new Choice(ma->T("YES"), new LayoutParams(200.0f, 64.0f)))->OnClick.Handle(this, &OffDialog::QuitMarley);
            items->Add(new Choice(ma->T("CANCEL"), new LayoutParams(200.0f, 64.0f)))->OnClick.Handle<SCREEN_UIScreen>(this, &SCREEN_UIScreen::OnBack);
        }
        else
        {    
            items->Add(new Choice(ma->T("YES"), new LayoutParams(200.0f, 64.0f)))->OnClick.Handle(this, &OffDialog::SwitchOff);
            items->Add(new Choice(ma->T("CANCEL"), new LayoutParams(200.0f, 64.0f)))->OnClick.Handle<SCREEN_UIScreen>(this, &SCREEN_UIScreen::OnBack);
        }
    }

    parent->Add(items);
}

SCREEN_UI::EventReturn OffDialog::SwitchOff(SCREEN_UI::EventParams &e)
{
    Engine::m_Engine->Shutdown(Engine::SWITCH_OFF_COMPUTER);

    return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn OffDialog::QuitMarley(SCREEN_UI::EventParams &e)
{
    Engine::m_Engine->Shutdown();

    return SCREEN_UI::EVENT_DONE;
}
