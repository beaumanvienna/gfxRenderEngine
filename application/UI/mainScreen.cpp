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
#include "i18n.h"
#include "UI.h"
#include "UI/mainScreen.h"
#include "UI/settingsScreen.h"
#include "viewGroup.h"
#include "root.h"
#include "spritesheet.h"
#include "offDialog.h"
#include "drawBuffer.h"

void MainScreen::OnAttach()
{ 
    m_SpritesheetSettings.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_GEAR_R), 4 /* frames */);
    m_SpritesheetOff.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_OFF_R), 4 /* frames */);
    m_SpritesheetHome.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_HOME_R), 4 /* frames */);
    m_SpritesheetLines.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_LINES_R), 4 /* frames */);
    m_SpritesheetGrid.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_GRID_R), 4 /* frames */);
}

bool MainScreen::key(const SCREEN_KeyInput &key)
{
    if ( !(m_OffButton->HasFocus()) && (key.flags & KEY_DOWN) && ((key.keyCode==NKCODE_BACK) || (key.keyCode==NKCODE_ESCAPE))) {
        SCREEN_UI::SetFocusedView(m_OffButton);
        return true;       
    }
    if ( (m_OffButton->HasFocus()) && (key.flags & KEY_DOWN) && ((key.keyCode==NKCODE_BACK) || (key.keyCode==NKCODE_ESCAPE))) {
        SCREEN_UI::EventParams e{};
        e.v = m_OffButton;
        SCREEN_UIScreen::OnBack(e);
        return true;       
    } 
    return SCREEN_UIDialogScreen::key(key);
}

void MainScreen::CreateViews()
{
    using namespace SCREEN_UI;
    auto ma = GetI18NCategory("Main");

    root_ = new AnchorLayout(new LayoutParams(FILL_PARENT, FILL_PARENT));
    root_->SetTag("root_");

    LinearLayout *verticalLayout = new LinearLayout(ORIENT_VERTICAL, new LayoutParams(FILL_PARENT, FILL_PARENT));
    verticalLayout->SetTag("verticalLayout");
    verticalLayout->SetSpacing(0.0f);
    root_->Add(verticalLayout);
    
    float availableWidth = Engine::m_Engine->GetContextWidth();
    float availableHeight = Engine::m_Engine->GetContextHeight();
    float marginLeftRight = 100.0f;
    float marginUpDown = 100.0f;
    float iconWidth = 128.0f;
    float iconHeight = 128.0f;
    float romBrowserHeight = 400.0f;
    
    float verticalSpacer = availableHeight - 2 * marginUpDown - 2 * iconHeight - romBrowserHeight;

    verticalLayout->Add(new Spacer(marginUpDown));
    
    mainInfo_ = new MainInfoMessage(ALIGN_CENTER | FLAG_WRAP_TEXT, new AnchorLayoutParams(availableWidth - marginLeftRight - 500.0f, WRAP_CONTENT, marginLeftRight, 20.0f, NONE, NONE));
    root_->Add(mainInfo_);
    
    // top line
    LinearLayout *topline = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(FILL_PARENT, WRAP_CONTENT));
    topline->SetTag("topLine");
    verticalLayout->Add(topline);
    
    float horizontalSpacerTopline = availableWidth - 2 * marginLeftRight - 2 * iconWidth;
    topline->Add(new Spacer(horizontalSpacerTopline,0.0f));
    
    Sprite* icon;
    Sprite* icon_active;
    Sprite* icon_depressed;
    
    // settings button
    icon = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
    icon_active = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_FOCUSED);
    icon_depressed = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
    Choice* settingsButton;
    settingsButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconWidth));
    settingsButton->OnClick.Handle(this, &MainScreen::settingsClick);
    settingsButton->OnHighlight.Add([=](EventParams &e) 
    {
        if (!m_ToolTipsShown[MAIN_SETTINGS])
        {
            m_ToolTipsShown[MAIN_SETTINGS] = true;
            mainInfo_->Show(ma->T("Settings", "Settings"), e.v);
        }
        return SCREEN_UI::EVENT_CONTINUE;
    });
    topline->Add(settingsButton);
    
    // off button
    icon = m_SpritesheetOff.GetSprite(BUTTON_4_STATES_NOT_FOCUSED); 
    icon_active = m_SpritesheetOff.GetSprite(BUTTON_4_STATES_FOCUSED); 
    icon_depressed = m_SpritesheetOff.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED); 
    m_OffButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconHeight),true);
    m_OffButton->OnClick.Handle(this, &MainScreen::offClick);
    m_OffButton->OnHold.Handle(this, &MainScreen::offHold);
    m_OffButton->OnHighlight.Add([=](EventParams &e) 
    {
        if (!m_ToolTipsShown[MAIN_OFF])
        {
            m_ToolTipsShown[MAIN_OFF] = true;
            mainInfo_->Show(ma->T("Off", "Off: exit Marley; keep this button pressed to switch the computer off"), e.v);
        }
        return SCREEN_UI::EVENT_CONTINUE;
    });
    topline->Add(m_OffButton);
    
    verticalLayout->Add(new Spacer(verticalSpacer));
    
    // second line
    LinearLayout *secondLine = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(FILL_PARENT, WRAP_CONTENT));
    topline->SetTag("secondLine");
    verticalLayout->Add(secondLine);
    
    float horizontalSpacerSecondLine = marginLeftRight;
    secondLine->Add(new Spacer(horizontalSpacerSecondLine,0.0f));

    // home button
    icon = m_SpritesheetHome.GetSprite(BUTTON_4_STATES_NOT_FOCUSED); 
    icon_active = m_SpritesheetHome.GetSprite(BUTTON_4_STATES_FOCUSED); 
    icon_depressed = m_SpritesheetHome.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED); 
    Choice* homeButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconHeight),true);
    homeButton->OnHighlight.Add([=](EventParams &e)
    {
        if (!m_ToolTipsShown[MAIN_HOME])
        {
            m_ToolTipsShown[MAIN_HOME] = true;
            mainInfo_->Show(ma->T("Home", "Jump in file browser to home directory"), e.v);
        }
        return SCREEN_UI::EVENT_CONTINUE;
    });
    secondLine->Add(homeButton);
    
    LinearLayout *gamesPathViewFrame = new LinearLayout(ORIENT_VERTICAL, new LinearLayoutParams(FILL_PARENT, 128.0f));
    gamesPathViewFrame->Add(new Spacer(40.0f));
    
    m_GamesPathView = new TextView("/home/yo/Gaming", ALIGN_LEFT | ALIGN_VCENTER | FLAG_WRAP_TEXT, true, new LinearLayoutParams(WRAP_CONTENT, 50.0f));
    gamesPathViewFrame->Add(m_GamesPathView);
    
    if (gTheme == THEME_RETRO) 
    {
        m_GamesPathView->SetTextColor(RETRO_COLOR_FONT_FOREGROUND);
        m_GamesPathView->SetShadow(true);
    }
    secondLine->Add(gamesPathViewFrame);

    LOG_APP_INFO("UI: views for main screen created");
}

void MainScreen::onFinish(DialogResult result) 
{
    SCREEN_System_SendMessage("finish", "");
}

void MainScreen::update() 
{
    SCREEN_UIScreen::update();
    
    if ((gUpdateCurrentScreen) || (gUpdateMainScreen)) 
    {
        RecreateViews();
        gUpdateCurrentScreen = false;
        gUpdateMainScreen = false;
    }
}

SCREEN_UI::EventReturn MainScreen::HomeClick(SCREEN_UI::EventParams &e) 
{
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
