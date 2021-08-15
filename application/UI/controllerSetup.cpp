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

#include "controllerSetup.h"
#include "coreSettings.h"
#include "core.h"
#include "drawBuffer.h"
#include "input.h"
#include "viewGroup.h"

ControllerSetup::ControllerSetup(SpriteSheet* spritesheet, SCREEN_UI::LayoutParams *layoutParams)
        : LinearLayout(SCREEN_UI::ORIENT_VERTICAL, layoutParams), m_SpritesheetMarley(spritesheet)
{
    m_SpritesheetSettings.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_GEAR_R), 4 /* frames */);
    Refresh();
}

void ControllerSetup::Refresh()
{
    using namespace SCREEN_UI;

    float tabMarginLeftRight = 80.0f;
    float availableWidth  = Engine::m_Engine->GetContextWidth() - 2 * tabMarginLeftRight;
    float availableHeight = Engine::m_Engine->GetContextHeight();
    float marginLeftRight = 128.0f;
    float iconWidth = 128.0f;
    float iconHeight = 128.0f;
    float iconSpacer = 10.0f;
    float halfIconWidth  = iconWidth / 2;
    float halfIconHeight = iconHeight / 2;

    // Reset content
    Clear();

    bool controllerPlugged = Input::GetControllerCount();
    double verticalSpace = (availableHeight - 4 * iconHeight) / 2;

    if (!controllerPlugged)
    {
        Add(new Spacer(verticalSpace - halfIconHeight));
        TextView* noController = new TextView(" Please connect a controller", ALIGN_CENTER | FLAG_WRAP_TEXT, 
                                    true, new LinearLayoutParams(availableWidth, halfIconHeight));
        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            noController->SetTextColor(RETRO_COLOR_FONT_FOREGROUND);
            noController->SetShadow(true);
        }
        Add(noController);
    }

    Add(new Spacer(halfIconHeight));

    // first controller
    { 
        if (controllerPlugged)
        {
            LinearLayout *controllerHorizontal = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(FILL_PARENT,verticalSpace));
            Add(controllerHorizontal);

            // setup button
            LinearLayout *verticalLayout = new LinearLayout(ORIENT_VERTICAL, new LinearLayoutParams(iconHeight,verticalSpace));
            controllerHorizontal->Add(verticalLayout);

            Sprite* icon;
            Sprite* icon_active;
            Sprite* icon_depressed;

            // setup button
            Choice* setupButton;
            if (CoreSettings::m_UITheme == THEME_RETRO)
            {
                icon = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
                icon_active = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_FOCUSED);
                icon_depressed = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
                
                setupButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconWidth));
            }
            else 
            {
                icon = m_SpritesheetMarley->GetSprite(I_GEAR);
                setupButton = new Choice(icon, new LayoutParams(iconWidth, iconHeight));
            }

            setupButton->OnClick.Handle(this, &ControllerSetup::OnStartSetup1);

            verticalLayout->Add(new Spacer(20.0f,(verticalSpace-iconHeight)/2));
            verticalLayout->Add(setupButton);
            controllerHorizontal->Add(new Spacer(iconWidth));

            // text view 'instruction'
            LinearLayout *textViewLayout = new LinearLayout(ORIENT_VERTICAL, new LinearLayoutParams(availableWidth-verticalSpace-iconHeight, verticalSpace));
            controllerHorizontal->Add(textViewLayout);

            m_TextSetup1 = new TextView((Controller::m_ControllerConfiguration.GetControllerID() == Controller::FIRST_CONTROLLER) ? "press dpad up" : "Start controller setup (1)", ALIGN_VCENTER | ALIGN_HCENTER | FLAG_WRAP_TEXT, 
                                        true, new LinearLayoutParams(availableWidth-verticalSpace-iconHeight, verticalSpace));
            // text view 'skip button with return'
            m_TextSetup1b = new TextView(((Controller::m_ControllerConfiguration.GetControllerID() == Controller::FIRST_CONTROLLER)) ? "(or use ENTER to skip this button)" : "", ALIGN_VCENTER | ALIGN_HCENTER | FLAG_WRAP_TEXT, 
                                        true, new LinearLayoutParams(availableWidth-verticalSpace-iconHeight, halfIconHeight / 2));
            if (CoreSettings::m_UITheme == THEME_RETRO)
            {
                m_TextSetup1->SetTextColor(RETRO_COLOR_FONT_FOREGROUND);
                m_TextSetup1->SetShadow(true);
                m_TextSetup1b->SetTextColor(RETRO_COLOR_FONT_FOREGROUND);
                m_TextSetup1b->SetShadow(true);
            }
            textViewLayout->Add(m_TextSetup1);
            if (Controller::m_ControllerConfiguration.IsRunning()) textViewLayout->Add(m_TextSetup1b);
            controllerHorizontal->Add(new Spacer(halfIconWidth / 2));

            // controller pic
            LinearLayout *controllerImageLayout = new LinearLayout(ORIENT_VERTICAL, new LinearLayoutParams(FILL_PARENT,verticalSpace));

            Sprite* controllerSprite = m_SpritesheetMarley->GetSprite(I_CONTROLLER);
            controllerImageLayout->Add(new Spacer((verticalSpace - controllerSprite->GetHeightGUI()) / 2));

            ImageView* controllerImage = new ImageView(controllerSprite, new AnchorLayoutParams(controllerSprite->GetWidthGUI(), controllerSprite->GetHeightGUI()));

            controllerImageLayout->Add(controllerImage);
            controllerHorizontal->Add(controllerImageLayout);

        } else
        {
            Add(new Spacer(verticalSpace));
        }
    }

    Add(new Spacer(halfIconHeight));

    // second controller
    { 
        if (Input::GetControllerCount() >= 2)
        {
            LinearLayout *controllerHorizontal = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(FILL_PARENT,verticalSpace));
            Add(controllerHorizontal);

            // setup button
            LinearLayout *verticalLayout = new LinearLayout(ORIENT_VERTICAL, new LinearLayoutParams(iconHeight,verticalSpace));
            controllerHorizontal->Add(verticalLayout);

            Sprite* icon;
            Sprite* icon_active;
            Sprite* icon_depressed;

            // setup button
            Choice* setupButton;
            if (CoreSettings::m_UITheme == THEME_RETRO)
            {
                icon = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
                icon_active = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_FOCUSED);
                icon_depressed = m_SpritesheetSettings.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED);
                
                setupButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconWidth));
            }
            else 
            {
                icon = m_SpritesheetMarley->GetSprite(I_GEAR);
                setupButton = new Choice(icon, new LayoutParams(iconWidth, iconHeight));
            }

            setupButton->OnClick.Handle(this, &ControllerSetup::OnStartSetup2);

            verticalLayout->Add(new Spacer(20.0f,(verticalSpace-iconHeight)/2));
            verticalLayout->Add(setupButton);
            controllerHorizontal->Add(new Spacer(iconWidth));

            // text view 'instruction'
            LinearLayout *textViewLayout = new LinearLayout(ORIENT_VERTICAL, new LinearLayoutParams(availableWidth-verticalSpace-iconHeight, verticalSpace));
            controllerHorizontal->Add(textViewLayout);

            m_TextSetup2 = new TextView((Controller::m_ControllerConfiguration.GetControllerID() == Controller::SECOND_CONTROLLER) ? "press dpad up" : "Start controller setup (2)", ALIGN_VCENTER | ALIGN_HCENTER | FLAG_WRAP_TEXT, 
                                        true, new LinearLayoutParams(availableWidth-verticalSpace-iconHeight, verticalSpace));
            // text view 'skip button with return'
            m_TextSetup2b = new TextView(((Controller::m_ControllerConfiguration.GetControllerID() == Controller::SECOND_CONTROLLER)) ? "(or use ENTER to skip this button)" : "", ALIGN_VCENTER | ALIGN_HCENTER | FLAG_WRAP_TEXT, 
                                        true, new LinearLayoutParams(availableWidth-verticalSpace-iconHeight, halfIconHeight / 2));
            if (CoreSettings::m_UITheme == THEME_RETRO)
            {
                m_TextSetup2->SetTextColor(RETRO_COLOR_FONT_FOREGROUND);
                m_TextSetup2->SetShadow(true);
                m_TextSetup2b->SetTextColor(RETRO_COLOR_FONT_FOREGROUND);
                m_TextSetup2b->SetShadow(true);
            }
            textViewLayout->Add(m_TextSetup2);
            if (Controller::m_ControllerConfiguration.IsRunning()) textViewLayout->Add(m_TextSetup2b);
            controllerHorizontal->Add(new Spacer(halfIconWidth / 2));

            // controller pic
            LinearLayout *controllerImageLayout = new LinearLayout(ORIENT_VERTICAL, new LinearLayoutParams(FILL_PARENT,verticalSpace));

            Sprite* controllerSprite = m_SpritesheetMarley->GetSprite(I_CONTROLLER);
            controllerImageLayout->Add(new Spacer((verticalSpace - controllerSprite->GetHeightGUI()) / 2));
    
            ImageView* controllerImage = new ImageView(controllerSprite, new AnchorLayoutParams(controllerSprite->GetWidthGUI(), controllerSprite->GetHeightGUI()));

            controllerImageLayout->Add(controllerImage);
            controllerHorizontal->Add(controllerImageLayout);
        }
    }
}

void ControllerSetup::Update()
{
    static int controllerCount = Input::GetControllerCount();;
    static int controllerCountPrevious = Input::GetControllerCount();;

    controllerCount = Input::GetControllerCount();
    if (controllerCountPrevious != controllerCount)
    {
        Refresh();
    }
    controllerCountPrevious = controllerCount;

    SetControllerConfText();
    if (Controller::m_ControllerConfiguration.MappingCreated()) Controller::m_ControllerConfiguration.Reset();

    ViewGroup::Update();
}

SCREEN_UI::EventReturn ControllerSetup::OnStartSetup1(SCREEN_UI::EventParams &e)
{
    Input::StartControllerConfig(Controller::FIRST_CONTROLLER);

    return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn ControllerSetup::OnStartSetup2(SCREEN_UI::EventParams &e)
{
    Input::StartControllerConfig(Controller::SECOND_CONTROLLER);

    return SCREEN_UI::EVENT_DONE;
}

void ControllerSetup::SetControllerConfText()
{
    int controllerID = Controller::m_ControllerConfiguration.GetControllerID();
    bool update = Controller::m_ControllerConfiguration.UpdateControllerText();
    std::string text1 = Controller::m_ControllerConfiguration.GetText(ControllerConfiguration::TEXT1);
    std::string text2 = Controller::m_ControllerConfiguration.GetText(ControllerConfiguration::TEXT2);

    if (update)
    {
        if (controllerID == Controller::FIRST_CONTROLLER)
        {
            m_TextSetup1->SetText(text1);
            m_TextSetup1b->SetText(text2);
        }
        else if (controllerID == Controller::SECOND_CONTROLLER)
        {
            m_TextSetup2->SetText(text1);
            m_TextSetup2b->SetText(text2);
        }
        Controller::m_ControllerConfiguration.ResetControllerText();
    }
}
