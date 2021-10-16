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

#include <stdlib.h>

#include "marley/marley.h"
#include "core.h"
#include "i18n.h"
#include "marley/UI/UI.h"
#include "marley/UI/mainScreen.h"
#include "marley/UI/settingsScreen.h"
#include "marley/marley.h"
#include "root.h"
#include "spritesheet.h"
#include "marley/UI/offDialog.h"
#include "marley/UI/pauseDialog.h"
#include "drawBuffer.h"
#include "marley/appSettings.h"

namespace MarleyApp
{

    void MainScreen::OnAttach()
    {
        m_SpritesheetSettings.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_GEAR_R), 4 /* frames */);
        m_SpritesheetOff.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_OFF_R), 4 /* frames */);
        m_SpritesheetHome.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_HOME_R), 4 /* frames */);
        m_SpritesheetLines.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_LINES_R), 4 /* frames */);
        m_SpritesheetGrid.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_GRID_R), 4 /* frames */);

        m_LastGamePath = AppSettings::m_LastGamePath;
        m_ROMbrowser = nullptr;
    }

    void MainScreen::OnDetach()
    {
        if (m_ROMbrowser)
        {
            AppSettings::m_LastGamePath = m_ROMbrowser->GetPath();
        }
    }

    bool MainScreen::key(const SCREEN_KeyInput &key)
    {
        if (!m_OffButton->HasFocus())
        {
            if (key.flags & KEY_DOWN)
            {
                if ( (key.deviceId == DEVICE_ID_KEYBOARD && key.keyCode == ENGINE_KEY_ESCAPE) ||
                     (key.deviceId == DEVICE_ID_PAD_0    && key.keyCode == Controller::BUTTON_GUIDE) )
                {
                    if (Marley::m_GameState->GetEmulationMode() != GameState::PAUSED)
                    {
                        SCREEN_UI::SetFocusedView(m_OffButton);

                    }
                    return true;
                }
            }
        }
        if (m_OffButton->HasFocus())
        {
            if (key.flags & KEY_DOWN)
            {
                if ( (key.deviceId == DEVICE_ID_KEYBOARD && key.keyCode == ENGINE_KEY_ESCAPE) ||
                     (key.deviceId == DEVICE_ID_PAD_0    && key.keyCode == Controller::BUTTON_GUIDE) )
                {
                    {
                        Engine::m_Engine->Shutdown();
                        return true;
                    }
                }
            }
        }
        return SCREEN_UIDialogScreen::key(key);
    }

    void MainScreen::CreateViews()
    {
        auto ma = GetI18NCategory("Main");

        root_ = new SCREEN_UI::AnchorLayout(new SCREEN_UI::LayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::FILL_PARENT));
        root_->SetTag("root_");

        SCREEN_UI::LinearLayout *verticalLayout = new SCREEN_UI::LinearLayout(SCREEN_UI::ORIENT_VERTICAL, new SCREEN_UI::LayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::FILL_PARENT));
        verticalLayout->SetTag("verticalLayout");
        root_->Add(verticalLayout);

        float availableWidth = Engine::m_Engine->GetContextWidth();
        float availableHeight = Engine::m_Engine->GetContextHeight();
        float marginLeftRight = 128.0f;
        float marginUpDown = 128.0f;
        float iconWidth = 128.0f;
        float iconHeight = 128.0f;
        float iconSpacer = 10.0f;
        float fileBrowserHeight = 158.0f;
        float fileBrowserWidth = availableWidth - 2* marginLeftRight - 2 * iconWidth - 2* iconSpacer;

        float verticalSpacer = availableHeight - 2 * marginUpDown - 4 * iconHeight - fileBrowserHeight;

        m_MainInfo = new InfoMessage(ALIGN_CENTER | FLAG_WRAP_TEXT, new SCREEN_UI::AnchorLayoutParams(availableWidth - marginLeftRight * 3 - 2 * iconWidth - iconSpacer,
                                        SCREEN_UI::WRAP_CONTENT, marginLeftRight, 0.0f, SCREEN_UI::NONE, SCREEN_UI::NONE));
        root_->Add(m_MainInfo);

        verticalLayout->Add(new SCREEN_UI::Spacer(marginUpDown));

        // top line
        SCREEN_UI::LinearLayout *topline = new SCREEN_UI::LinearLayout(SCREEN_UI::ORIENT_HORIZONTAL, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::WRAP_CONTENT));
        topline->SetTag("topLine");
        verticalLayout->Add(topline);

        float horizontalSpacerTopline = availableWidth - marginLeftRight - 2 * iconWidth - iconSpacer;
        topline->Add(new SCREEN_UI::Spacer(horizontalSpacerTopline,0.0f));

        Sprite* icon;
        Sprite* icon_active;
        Sprite* icon_depressed;

        // settings button
        SCREEN_UI::Choice* settingsButton;
        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            icon = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            icon_active = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_FOCUSED);
            icon_depressed = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);

            settingsButton = new SCREEN_UI::Choice(icon, icon_active, icon_depressed, new SCREEN_UI::LayoutParams(iconWidth, iconWidth));
        }
        else
        {
            icon = m_SpritesheetMarley->GetSprite(I_GEAR);
            settingsButton = new SCREEN_UI::Choice(icon, new SCREEN_UI::LayoutParams(iconWidth, iconHeight));
        }

        settingsButton->OnClick.Handle(this, &MainScreen::settingsClick);
        settingsButton->OnHighlight.Add([=](SCREEN_UI::EventParams &e)
        {
            if (!m_ToolTipsShown[MAIN_SETTINGS])
            {
                m_ToolTipsShown[MAIN_SETTINGS] = true;
                m_MainInfo->Show(ma->T("Settings", "Settings"), e.v);
            }
            return SCREEN_UI::EVENT_CONTINUE;
        });
        topline->Add(settingsButton);
        topline->Add(new SCREEN_UI::Spacer(iconSpacer,0.0f));

        // off button
        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            icon = m_SpritesheetOff.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            icon_active = m_SpritesheetOff.GetSprite(BUTTON_4_STATES_FOCUSED);
            icon_depressed = m_SpritesheetOff.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
            m_OffButton = new SCREEN_UI::Choice(icon, icon_active, icon_depressed, new SCREEN_UI::LayoutParams(iconWidth, iconHeight),true);
        }
        else
        {
            icon = m_SpritesheetMarley->GetSprite(I_OFF);
            m_OffButton = new SCREEN_UI::Choice(icon, new SCREEN_UI::LayoutParams(iconWidth, iconHeight), true);
        }
        m_OffButton->OnClick.Handle(this, &MainScreen::offClick);
        m_OffButton->OnHold.Handle(this, &MainScreen::offHold);
        m_OffButton->OnHighlight.Add([=](SCREEN_UI::EventParams &e)
        {
            if (!m_ToolTipsShown[MAIN_OFF])
            {
                m_ToolTipsShown[MAIN_OFF] = true;
                m_MainInfo->Show(ma->T("Off", "Off: exit Marley; keep this button pressed to switch the computer off"), e.v);
            }
            return SCREEN_UI::EVENT_CONTINUE;
        });
        topline->Add(m_OffButton);

        verticalLayout->Add(new SCREEN_UI::Spacer(verticalSpacer));

        // -------- horizontal main launcher frame --------
        SCREEN_UI::LinearLayout *gameLauncherMainFrame = new SCREEN_UI::LinearLayout(SCREEN_UI::ORIENT_HORIZONTAL, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, 273.0f,1.0f));
        verticalLayout->Add(gameLauncherMainFrame);
        gameLauncherMainFrame->SetTag("gameLauncherMainFrame");
        gameLauncherMainFrame->Add(new SCREEN_UI::Spacer(marginLeftRight));

        // vertical layout for the game browser's top bar and the scroll view
        SCREEN_UI::Margins mgn(0,0,0,0);
        SCREEN_UI::LinearLayout *gameLauncherColumn = new SCREEN_UI::LinearLayout(SCREEN_UI::ORIENT_VERTICAL, new SCREEN_UI::LinearLayoutParams(fileBrowserWidth, 243.0f, 0.0f,SCREEN_UI::G_TOPLEFT, mgn));
        gameLauncherMainFrame->Add(gameLauncherColumn);
        gameLauncherColumn->SetTag("gameLauncherColumn");

        // game browser's top bar
        SCREEN_UI::LinearLayout *topBar = new SCREEN_UI::LinearLayout(SCREEN_UI::ORIENT_HORIZONTAL, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::WRAP_CONTENT));
        gameLauncherColumn->Add(topBar);
        topBar->SetTag("topBar");

        // home button
        SCREEN_UI::Choice* homeButton;
        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            icon = m_SpritesheetHome.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            icon_active = m_SpritesheetHome.GetSprite(BUTTON_4_STATES_FOCUSED);
            icon_depressed = m_SpritesheetHome.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
            homeButton = new SCREEN_UI::Choice(icon, icon_active, icon_depressed, new SCREEN_UI::LayoutParams(iconWidth, iconHeight),true);
        }
        else
        {
            icon = m_SpritesheetMarley->GetSprite(I_HOME);
            homeButton = new SCREEN_UI::Choice(icon, new SCREEN_UI::LayoutParams(iconWidth, iconHeight));
        }
        homeButton->OnHighlight.Add([=](SCREEN_UI::EventParams &e)
        {
            if (!m_ToolTipsShown[MAIN_HOME])
            {
                m_ToolTipsShown[MAIN_HOME] = true;
                m_MainInfo->Show(ma->T("Home", "Jump in file browser to home directory"), e.v);
            }
            return SCREEN_UI::EVENT_CONTINUE;
        });
        homeButton->OnClick.Handle(this, &MainScreen::HomeClick);
        topBar->Add(homeButton);
        topBar->Add(new SCREEN_UI::Spacer(iconSpacer,0.0f));
        SCREEN_UI::LinearLayout *gamesPathViewFrame = new SCREEN_UI::LinearLayout(SCREEN_UI::ORIENT_VERTICAL, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, 128.0f));
        gamesPathViewFrame->Add(new SCREEN_UI::Spacer(40.0f));

        m_GamesPathView = new SCREEN_UI::TextView(m_LastGamePath, ALIGN_LEFT | ALIGN_VCENTER | FLAG_WRAP_TEXT, true, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::WRAP_CONTENT, 50.0f));
        gamesPathViewFrame->Add(m_GamesPathView);

        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            m_GamesPathView->SetTextColor(RETRO_COLOR_FONT_FOREGROUND);
            m_GamesPathView->SetShadow(true);
        }
        topBar->Add(gamesPathViewFrame);

        gameLauncherColumn->Add(new SCREEN_UI::Spacer(50.0f));

        // frame for scolling
        m_GameLauncherFrameScroll = new SCREEN_UI::ScrollView(SCREEN_UI::ORIENT_VERTICAL, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, fileBrowserHeight),true);
        gameLauncherColumn->Add(m_GameLauncherFrameScroll);

        // game browser
        m_ROMbrowser = new ROMBrowser
        (
            m_LastGamePath,
            m_GamesPathView,
            new SCREEN_UI::LinearLayoutParams(fileBrowserWidth, SCREEN_UI::WRAP_CONTENT)
        );
        m_ROMbrowser->SetTag("m_ROMbrowser");
        m_GameLauncherFrameScroll->Add(m_ROMbrowser);

        m_ROMbrowser->OnHoldChoice.Handle(this, &MainScreen::OnROMBrowserHoldChoice);
        m_ROMbrowser->OnROMClick.Handle(this, &MainScreen::OnROMClick);
        m_ROMbrowser->OnNavigateClick.Handle(this, &MainScreen::OnROMBrowserNavigateClick);
        m_ROMbrowser->SetEventCallback([](AppEvent& event){ return Marley::m_Application->OnAppEvent(event);});


        root_->SetDefaultFocusView(m_ROMbrowser);
        if (m_ROMbrowser->GetDefaultFocusView())
        {
            SCREEN_UI::SetFocusedView(m_ROMbrowser->GetDefaultFocusView());
        }

        LOG_APP_INFO("UI: views for main screen created");
    }

    SCREEN_UI::EventReturn MainScreen::OnROMBrowserHoldChoice(SCREEN_UI::EventParams &e)
    {
        return SCREEN_UI::EVENT_DONE;
    }

    SCREEN_UI::EventReturn MainScreen::OnROMClick(SCREEN_UI::EventParams &e)
    {
        return SCREEN_UI::EVENT_DONE;
    }

    SCREEN_UI::EventReturn MainScreen::OnROMBrowserNavigateClick(SCREEN_UI::EventParams &e)
    {
        m_GameLauncherFrameScroll->ScrollTo(0.0f);
        return SCREEN_UI::EVENT_DONE;
    }

    void MainScreen::onFinish(DialogResult result)
    {
        SCREEN_System_SendMessage("finish", "");
    }

    void MainScreen::update()
    {
        if (Marley::m_GameState->GetEmulationMode() == GameState::PAUSED)
        {
            SCREEN_UI::EventParams event;
            EmulationPaused(event);
        }
        SCREEN_UIScreen::update();
    }

    SCREEN_UI::EventReturn MainScreen::HomeClick(SCREEN_UI::EventParams &e)
    {
        m_LastGamePath = Engine::m_Engine->GetHomeDirectory();

        m_ROMbrowser->SetPath(m_LastGamePath);
        if (m_ROMbrowser->GetDefaultFocusView())
        {
            SCREEN_UI::SetFocusedView(m_ROMbrowser->GetDefaultFocusView());
        }
        return SCREEN_UI::EVENT_DONE;
    }

    SCREEN_UI::EventReturn MainScreen::settingsClick(SCREEN_UI::EventParams &e)
    {
        SettingsScreen* settingsScreen = new SettingsScreen(m_SpritesheetMarley);
        settingsScreen->OnAttach();
        UI::m_ScreenManager->push(settingsScreen);

        return SCREEN_UI::EVENT_DONE;
    }

    SCREEN_UI::EventReturn MainScreen::offClick(SCREEN_UI::EventParams &e)
    {
        auto ma = GetI18NCategory("System");
        auto offClick = new OffDialog(ma->T("Exit Marley?"), OFFDIAG_QUIT);
        if (e.v)
        {
            offClick->SetPopupOrigin(e.v);
        }

        screenManager()->push(offClick);

        return SCREEN_UI::EVENT_DONE;
    }

    SCREEN_UI::EventReturn MainScreen::EmulationPaused(SCREEN_UI::EventParams &e)
    {
        auto ma = GetI18NCategory("System");
        auto emulationPaused = new PauseDialog(ma->T("Exit game?"));
        if (e.v)
        {
            emulationPaused->SetPopupOrigin(e.v);
        }

        screenManager()->push(emulationPaused);

        return SCREEN_UI::EVENT_DONE;
    }

    SCREEN_UI::EventReturn MainScreen::offHold(SCREEN_UI::EventParams &e)
    {
        auto ma = GetI18NCategory("System");
        auto offDiag = new OffDialog(ma->T("Switch off computer?"), OFFDIAG_SHUTDOWN);
        if (e.v)
        {
            offDiag->SetPopupOrigin(e.v);
        }

        screenManager()->push(offDiag);

        return SCREEN_UI::EVENT_DONE;
    }
}
