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
#include "i18n.h"
#include "UI/mainScreen.h"
#include "viewGroup.h"
#include "root.h"
#include "spritesheet.h"

void MainScreen::OnAttach()
{ 
    m_SpritesheetSettings.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_GEAR_R), 4 /* frames */);
    m_SpritesheetOff.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_OFF_R), 4 /* frames */);
}

bool MainScreen::key(const SCREEN_KeyInput &key)
{
    if ( !(offButton->HasFocus()) && (key.flags & KEY_DOWN) && ((key.keyCode==NKCODE_BACK) || (key.keyCode==NKCODE_ESCAPE))) {
        SCREEN_UI::SetFocusedView(offButton);
        return true;       
    }
    if ( (offButton->HasFocus()) && (key.flags & KEY_DOWN) && ((key.keyCode==NKCODE_BACK) || (key.keyCode==NKCODE_ESCAPE))) {
        SCREEN_UI::EventParams e{};
        e.v = offButton;
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

    verticalLayout->Add(new Spacer(f10));
    
    // top line
    LinearLayout *topline = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(FILL_PARENT, WRAP_CONTENT));
    topline->SetTag("topLine");
    verticalLayout->Add(topline);
    
    topline->Add(new Spacer(100.0f,0.0f));
    
    Sprite* icon;
    Sprite* icon_active;
    Sprite* icon_depressed;
    
    // settings button
    icon = m_SpritesheetSettings.GetSprite(BUTTON_STATE_NOT_FOCUSED);
    icon_active = m_SpritesheetSettings.GetSprite(BUTTON_STATE_FOCUSED);
    icon_depressed = m_SpritesheetSettings.GetSprite(BUTTON_STATE_FOCUSED_DEPRESSED);
    Choice* settingsButton;
    settingsButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(f128, f128));
    
    settingsButton->OnClick.Handle(this, &MainScreen::settingsClick);
    settingsButton->OnHighlight.Add([=](EventParams &e) 
    {
        return SCREEN_UI::EVENT_CONTINUE;
    });
    topline->Add(settingsButton);
    
    // off button
    icon = m_SpritesheetOff.GetSprite(BUTTON_STATE_NOT_FOCUSED); 
    icon_active = m_SpritesheetOff.GetSprite(BUTTON_STATE_FOCUSED); 
    icon_depressed = m_SpritesheetOff.GetSprite(BUTTON_STATE_FOCUSED_DEPRESSED); 
    offButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(f128, f128),true);
    
    offButton->OnClick.Handle(this, &MainScreen::offClick);
    offButton->OnHold.Handle(this, &MainScreen::offHold);
    offButton->OnHighlight.Add([=](EventParams &e) 
    {
        return SCREEN_UI::EVENT_CONTINUE;
    });
    topline->Add(offButton);

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
    return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn MainScreen::offClick(SCREEN_UI::EventParams &e) 
{    
    return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn MainScreen::offHold(SCREEN_UI::EventParams &e) 
{
    return SCREEN_UI::EVENT_DONE;
}
