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

        LinearLayout *items = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(WRAP_CONTENT, WRAP_CONTENT));

        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            float iconWidth = 128.0f;
            float iconHeight = 128.0f;
            
            items->Add(new Spacer(9.0f));

            //back button
            m_SpritesheetBack.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_BACK_R), 4 /* frames */);

            m_Icon           = m_SpritesheetBack.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            m_Icon_active    = m_SpritesheetBack.GetSprite(BUTTON_4_STATES_FOCUSED);
            m_Icon_depressed = m_SpritesheetBack.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
            m_BackButton = new Choice(m_Icon, m_Icon_active, m_Icon_depressed, new LayoutParams(iconWidth, iconHeight));

            m_BackButton->OnClick.Handle(this, &PauseDialog::Return);

            items->Add(m_BackButton);

            //save button
            m_SpritesheetSave.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_DISK_SAVE_R), 4 /* frames */);

            m_Icon           = m_SpritesheetSave.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            m_Icon_active    = m_SpritesheetSave.GetSprite(BUTTON_4_STATES_FOCUSED);
            m_Icon_depressed = m_SpritesheetSave.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
            m_SaveButton = new Choice(m_Icon, m_Icon_active, m_Icon_depressed, new LayoutParams(iconWidth, iconHeight));

            m_SaveButton->OnClick.Handle(this, &PauseDialog::Return);

            items->Add(m_SaveButton);

            //load button
            m_SpritesheetLoad.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_DISK_LOAD_R), 4 /* frames */);

            m_Icon           = m_SpritesheetLoad.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            m_Icon_active    = m_SpritesheetLoad.GetSprite(BUTTON_4_STATES_FOCUSED);
            m_Icon_depressed = m_SpritesheetLoad.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
            m_LoadButton = new Choice(m_Icon, m_Icon_active, m_Icon_depressed, new LayoutParams(iconWidth, iconHeight));

            m_LoadButton->OnClick.Handle(this, &PauseDialog::Return);

            items->Add(m_LoadButton);

            // off button
            m_SpritesheetOff.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_OFF_R), 4 /* frames */);

            m_Icon           = m_SpritesheetOff.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            m_Icon_active    = m_SpritesheetOff.GetSprite(BUTTON_4_STATES_FOCUSED);
            m_Icon_depressed = m_SpritesheetOff.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
            m_OffButton = new Choice(m_Icon, m_Icon_active, m_Icon_depressed, new LayoutParams(iconWidth, iconHeight),true);

            m_OffButton->OnClick.Handle(this, &PauseDialog::ExitEmulation);

            items->Add(m_OffButton);
        }
        else
        {

            m_BackButton = new Choice(ma->T("Return"), new LayoutParams(132.0f, 64.0f));
            m_SaveButton = new Choice(ma->T("Save"), new LayoutParams(132.0f, 64.0f));
            m_LoadButton = new Choice(ma->T("Load"), new LayoutParams(132.0f, 64.0f));
            m_OffButton    = new Choice(ma->T("Exit"), new LayoutParams(132.0f, 64.0f));

            m_BackButton->OnClick.Handle(this, &PauseDialog::Return);
            m_SaveButton->OnClick.Handle(this, &PauseDialog::Save);
            m_LoadButton->OnClick.Handle(this, &PauseDialog::Load);
            m_OffButton->OnClick.Handle(this, &PauseDialog::ExitEmulation);

            m_BackButton->SetCentered(true);
            m_SaveButton->SetCentered(true);
            m_LoadButton->SetCentered(true);
            m_OffButton->SetCentered(true);

            items->Add(m_BackButton);
            items->Add(m_SaveButton);
            items->Add(m_LoadButton);
            items->Add(m_OffButton);
        }

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

    SCREEN_UI::EventReturn PauseDialog::Save(SCREEN_UI::EventParams &e)
    {
        Marley::m_GameState->SetEmulationMode(GameState::RUNNING);

        return SCREEN_UIScreen::OnBack(e);
    }

    SCREEN_UI::EventReturn PauseDialog::Load(SCREEN_UI::EventParams &e)
    {
        Marley::m_GameState->SetEmulationMode(GameState::RUNNING);

        return SCREEN_UIScreen::OnBack(e);
    }
}
