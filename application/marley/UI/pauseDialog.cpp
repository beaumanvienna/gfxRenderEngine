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

        Sprite* icon;
        Sprite* icon_active;
        Sprite* icon_depressed;

        Choice* backButton;
        Choice* offButton;
        Choice* saveButton;
        Choice* loadButton;

        LinearLayout *items = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(WRAP_CONTENT, WRAP_CONTENT));

        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            float iconWidth = 128.0f;
            float iconHeight = 128.0f;
            
            items->Add(new Spacer(9.0f));

            //back button
            SpriteSheet spritesheetBack;
            spritesheetBack.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_BACK_R), 4 /* frames */);

            icon           = spritesheetBack.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            icon_active    = spritesheetBack.GetSprite(BUTTON_4_STATES_FOCUSED);
            icon_depressed = spritesheetBack.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
            backButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconHeight));

            backButton->OnClick.Handle(this, &PauseDialog::Return);

            items->Add(backButton);

            //save button
            SpriteSheet spritesheetSave;
            spritesheetSave.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_DISK_SAVE_R), 4 /* frames */);

            icon           = spritesheetSave.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            icon_active    = spritesheetSave.GetSprite(BUTTON_4_STATES_FOCUSED);
            icon_depressed = spritesheetSave.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
            saveButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconHeight));

            saveButton->OnClick.Handle(this, &PauseDialog::Return);

            items->Add(saveButton);

            //load button
            SpriteSheet spritesheetLoad;
            spritesheetLoad.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_DISK_LOAD_R), 4 /* frames */);

            icon           = spritesheetLoad.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            icon_active    = spritesheetLoad.GetSprite(BUTTON_4_STATES_FOCUSED);
            icon_depressed = spritesheetLoad.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
            loadButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconHeight));

            loadButton->OnClick.Handle(this, &PauseDialog::Return);

            items->Add(loadButton);

            // off button
            SpriteSheet spritesheetOff;
            spritesheetOff.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_OFF_R), 4 /* frames */);

            icon           = spritesheetOff.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            icon_active    = spritesheetOff.GetSprite(BUTTON_4_STATES_FOCUSED);
            icon_depressed = spritesheetOff.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
            offButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconHeight),true);

            offButton->OnClick.Handle(this, &PauseDialog::ExitEmulation);

            items->Add(offButton);
        }
        else
        {
            Choice* returnButton;
            Choice* saveButton;
            Choice* loadButton;
            Choice* exitButton;

            returnButton = new Choice(ma->T("Return"), new LayoutParams(132.0f, 64.0f));
            saveButton = new Choice(ma->T("Save"), new LayoutParams(132.0f, 64.0f));
            loadButton = new Choice(ma->T("Load"), new LayoutParams(132.0f, 64.0f));
            exitButton    = new Choice(ma->T("Exit"), new LayoutParams(132.0f, 64.0f));

            returnButton->OnClick.Handle(this, &PauseDialog::Return);
            saveButton->OnClick.Handle(this, &PauseDialog::Save);
            loadButton->OnClick.Handle(this, &PauseDialog::Load);
            exitButton->OnClick.Handle(this, &PauseDialog::ExitEmulation);

            returnButton->SetCentered(true);
            saveButton->SetCentered(true);
            loadButton->SetCentered(true);
            exitButton->SetCentered(true);

            items->Add(returnButton);
            items->Add(saveButton);
            items->Add(loadButton);
            items->Add(exitButton);
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
