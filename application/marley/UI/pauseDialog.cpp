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
#include "marley/UI/pauseDialog.h"
#include "marley/marley.h"
#include "i18n.h"
#include "viewGroup.h"

#define TRANSPARENT_BACKGROUND true

namespace MarleyApp
{

    void PauseDialog::CreatePopupContents(SCREEN_UI::ViewGroup *parent)
    {
        using namespace SCREEN_UI;

        auto ma = GetI18NCategory("Main");

        Choice* yesButton;
        Choice* cancelButton;

        LinearLayout *items = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(WRAP_CONTENT, WRAP_CONTENT));

        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            yesButton    = new  Choice(ma->T("YES"), TRANSPARENT_BACKGROUND, new LayoutParams(265.0f, 64.0f));
            cancelButton = new  Choice(ma->T("CANCEL"), TRANSPARENT_BACKGROUND, new LayoutParams(265.0f, 64.0f));
            yesButton->OnClick.Handle(this, &PauseDialog::ExitEmulation);
            cancelButton->OnClick.Handle(this, &PauseDialog::Return);
        } else
        {            
            yesButton    = new Choice(ma->T("YES"), new LayoutParams(265.0f, 64.0f));
            cancelButton = new Choice(ma->T("CANCEL"), new LayoutParams(265.0f, 64.0f));
            yesButton->OnClick.Handle(this, &PauseDialog::Return);
            cancelButton->OnClick.Handle<SCREEN_UIScreen>(this, &SCREEN_UIScreen::OnBack);
        }

        yesButton->SetCentered(true);
        cancelButton->SetCentered(true);

        items->Add(yesButton);
        items->Add(cancelButton);

        parent->Add(items);
    }

    SCREEN_UI::EventReturn PauseDialog::ExitEmulation(SCREEN_UI::EventParams &e)
    {
        Marley::m_GameState->ExitEmulation();

        return SCREEN_UIScreen::OnBack(e);
    }

    SCREEN_UI::EventReturn PauseDialog::Return(SCREEN_UI::EventParams &e)
    {
        Marley::m_GameState->SetEmulationMode(GameState::RUNNING);

        return SCREEN_UIScreen::OnBack(e);
    }
}
