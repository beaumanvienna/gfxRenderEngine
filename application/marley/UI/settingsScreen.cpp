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

#include "core.h"
#include "root.h"
#include "common.h"
#include "drawBuffer.h"
#include "marley/UI/UI.h"
#include "instrumentation.h"
#include "marley/UI/settingsScreen.h"
#include "marley/UI/browser/directoryBrowser.h"
#include "marley/marley.h"
#include "spritesheet.h"
#include "viewGroup.h"
#include "sound.h"
#include "i18n.h"

namespace MarleyApp
{

    bool SettingsScreen::m_IsCreditsScreen = false;
    bool SettingsScreen::m_IsCintrollerSetupScreen = false;

    SettingsScreen::~SettingsScreen()
    {
        m_IsCreditsScreen = false;
        m_IsCintrollerSetupScreen = false;
    }

    void SettingsScreen::OnAttach()
    { 
        m_SpritesheetTab.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_TAB_R), 2 /* frames */, TAB_SCALE);
        m_SpritesheetBack.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_BACK_R), 4 /* frames */);
        m_LastTab = 0;
        m_BiosDirBrowser = nullptr;
    }

    bool SettingsScreen::key(const SCREEN_KeyInput &key)
    {
        if (m_ControllerSetup->IsRunning())
        {
            if (key.keyCode == ENGINE_KEY_ENTER)
            {
                m_ControllerSetup->Key(key);
            }
            else if (key.keyCode == ENGINE_KEY_ESCAPE)
            {
                return SCREEN_UIDialogScreen::key(key);
            }
            return false;
        }
        else
        {
            return SCREEN_UIDialogScreen::key(key);
        }
    }

    void SettingsScreen::CreateViews()
    {
        PROFILE_FUNCTION();
        using namespace SCREEN_UI;
        auto ge  = GetI18NCategory("Bios");
        auto ps2 = GetI18NCategory("PCSX2");
        auto dol = GetI18NCategory("Dolphin");

        root_ = new AnchorLayout(new LayoutParams(FILL_PARENT, FILL_PARENT));
        root_->SetTag("setting screen root");

        LinearLayout *verticalLayout = new LinearLayout(ORIENT_VERTICAL, new LayoutParams(FILL_PARENT, FILL_PARENT));
        verticalLayout->SetTag("main verticalLayout settings screen");
        root_->Add(verticalLayout);

        float availableWidth = Engine::m_Engine->GetContextWidth();
        float availableHeight = Engine::m_Engine->GetContextHeight();

        float iconWidth = 128.0f;
        float iconHeight = 128.0f;
        float stripSize = 204.0f * TAB_SCALE;
        float tabMargin = (availableWidth - 6 * stripSize) / 2.0f;
        float tabMarginLeftRight = 80.0f;
        float tabLayoutWidth = availableWidth - 2 * tabMarginLeftRight;

        // info message
        m_SettingsInfo = new InfoMessage(ALIGN_CENTER | FLAG_WRAP_TEXT, new AnchorLayoutParams(availableWidth - 6 * iconWidth, WRAP_CONTENT, 4 * iconWidth, 0.0f, NONE, NONE));
        m_SettingsInfo->SetBottomCutoff(availableHeight - iconHeight);

        root_->Add(m_SettingsInfo);

        verticalLayout->Add(new Spacer(tabMargin));

        m_TabHolder = new TabHolder(ORIENT_HORIZONTAL, stripSize, new LinearLayoutParams(1.0f), tabMargin);
        m_TabHolder->SetTag("m_TabHolder");
        verticalLayout->Add(m_TabHolder);

        if (CoreSettings::m_UITheme == THEME_RETRO)
        { 
            Sprite* icon;
            Sprite* icon_active;
            Sprite* icon_depressed;
            Sprite* icon_depressed_inactive;

            icon = m_SpritesheetTab.GetSprite(BUTTON_2_STATES_NOT_FOCUSED);
            icon_active = m_SpritesheetTab.GetSprite(BUTTON_2_STATES_FOCUSED);
            icon_depressed = m_SpritesheetTab.GetSprite(BUTTON_2_STATES_FOCUSED);
            icon_depressed_inactive = m_SpritesheetTab.GetSprite(BUTTON_2_STATES_NOT_FOCUSED);
            m_TabHolder->SetIcon(icon,icon_active,icon_depressed,icon_depressed_inactive);
        }

        // back button
        Choice* backButton;
        LinearLayout *horizontalLayoutBack = new LinearLayout(ORIENT_HORIZONTAL, new LayoutParams(FILL_PARENT, iconHeight));
        horizontalLayoutBack->SetTag("horizontalLayoutBack");
        if (CoreSettings::m_UITheme == THEME_RETRO)
        {
            Sprite* icon = m_SpritesheetBack.GetSprite(BUTTON_4_STATES_NOT_FOCUSED);
            Sprite* icon_active = m_SpritesheetBack.GetSprite(BUTTON_4_STATES_FOCUSED); 
            Sprite* icon_depressed = m_SpritesheetBack.GetSprite(BUTTON_4_STATES_FOCUSED_DEPRESSED); 
            backButton = new Choice(icon, icon_active, icon_depressed, new LayoutParams(iconWidth, iconHeight));
        }
        else
        {
            Sprite* icon = m_SpritesheetMarley->GetSprite(I_BACK);
            backButton = new Choice(icon, new LayoutParams(iconWidth, iconHeight));
        }
        backButton->SetTag("backButton");
        backButton->OnClick.Handle<SCREEN_UIScreen>(this, &SCREEN_UIScreen::OnBack);
        horizontalLayoutBack->Add(new Spacer(40.0f));
        horizontalLayoutBack->Add(backButton);
        verticalLayout->Add(horizontalLayoutBack);
        verticalLayout->Add(new Spacer(40.0f));

        root_->SetDefaultFocusView(m_TabHolder);

        // -------- bios --------

        // horizontal layout for margins
        LinearLayout *horizontalLayoutBios = new LinearLayout(ORIENT_HORIZONTAL, new LayoutParams(tabLayoutWidth, FILL_PARENT));
        horizontalLayoutBios->SetTag("horizontalLayoutBios");
        m_TabHolder->AddTab(ge->T("Bios"), horizontalLayoutBios);
        horizontalLayoutBios->Add(new Spacer(tabMarginLeftRight));
        LinearLayout *verticalLayoutBios = new LinearLayout(ORIENT_VERTICAL);
        horizontalLayoutBios->Add(verticalLayoutBios);
        
        SCREEN_UI::TextView* usageBiosSearchBrowser;
        usageBiosSearchBrowser = new TextView("Find bios files: select a folder and hit controller->START or SPACE",     ALIGN_LEFT, true, new LinearLayoutParams(WRAP_CONTENT, WRAP_CONTENT, 1.0f));
        verticalLayoutBios->Add(new Spacer(10.0f));
        verticalLayoutBios->Add(usageBiosSearchBrowser);

        //bios file browser
        m_BiosDirBrowser = new DirectoryBrowser
        (
            Engine::m_Engine->GetHomeDirectory(),
            DirectoryBrowserFlags::STANDARD,
            screenManager(),
            ge->T("Use the Start button to confirm"),
            m_SpritesheetMarley,
            new LinearLayoutParams(FILL_PARENT, FILL_PARENT)
        );
        verticalLayoutBios->Add(m_BiosDirBrowser);
        verticalLayoutBios->Add(new Spacer(32.0f));
        
        Marley::m_BiosFiles.CheckBiosFiles();
        
        verticalLayoutBios->Add(BiosInfo("PS1 bios file for Japan",                         Marley::m_BiosFiles.m_BiosFoundPS1Japan));
        verticalLayoutBios->Add(BiosInfo("PS1 bios file for North America",                 Marley::m_BiosFiles.m_BiosFoundPS1NorthAmerica));
        verticalLayoutBios->Add(BiosInfo("PS1 bios file for Europe",                        Marley::m_BiosFiles.m_BiosFoundPS1Europe));
        verticalLayoutBios->Add(BiosInfo("PS2 bios file for Japan",                         Marley::m_BiosFiles.m_BiosFoundPS2Japan));
        verticalLayoutBios->Add(BiosInfo("PS2 bios file for North America",                 Marley::m_BiosFiles.m_BiosFoundPS2NorthAmerica));
        verticalLayoutBios->Add(BiosInfo("PS2 bios file for Europe",                        Marley::m_BiosFiles.m_BiosFoundPS2Europe));
        verticalLayoutBios->Add(BiosInfo("Sega Saturn bios file Japan",                     Marley::m_BiosFiles.m_BiosFoundSegaSaturnJapan));
        verticalLayoutBios->Add(BiosInfo("Sega Saturn bios file North America and Europe",  Marley::m_BiosFiles.m_BiosFoundSegaSaturnNorthAmericaEurope));

        // -------- controller setup --------

        // horizontal layout for margins
        LinearLayout *horizontalLayoutController = new LinearLayout(ORIENT_HORIZONTAL, new LayoutParams(tabLayoutWidth, FILL_PARENT));
        horizontalLayoutController->SetTag("horizontalLayoutController");
        m_TabHolder->AddTab(ge->T("Controller"), horizontalLayoutController);

        horizontalLayoutController->Add(new Spacer(tabMarginLeftRight));

        m_ControllerSetup = new ControllerSetup(m_SpritesheetMarley);
        horizontalLayoutController->Add(m_ControllerSetup);
        m_ControllerSetup->OnMappingSuccessful.Add([=](EventParams &e) 
        {
            m_SettingsInfo->Show("Mapping successful", e.v);
            return SCREEN_UI::EVENT_CONTINUE;
        });

        // -------- Dolphin --------

        // horizontal layout for margins
        LinearLayout *horizontalLayoutDolphin = new LinearLayout(ORIENT_HORIZONTAL, new LayoutParams(tabLayoutWidth, FILL_PARENT));
        horizontalLayoutDolphin->SetTag("horizontalLayoutDolphin");
        m_TabHolder->AddTab(ge->T("Dolphin"), horizontalLayoutDolphin);
        horizontalLayoutDolphin->Add(new Spacer(tabMarginLeftRight));

        ViewGroup *dolphinSettingsScroll = new ScrollView(ORIENT_VERTICAL, new LinearLayoutParams(tabLayoutWidth, FILL_PARENT));
        dolphinSettingsScroll->SetTag("dolphinSettingsScroll");
        horizontalLayoutDolphin->Add(dolphinSettingsScroll);

        LinearLayout *dolphinSettings = new LinearLayout(ORIENT_VERTICAL);
        dolphinSettings->SetTag("DolphinSettings");
        dolphinSettings->Add(new Spacer(10.0f));
        dolphinSettingsScroll->Add(dolphinSettings);

        // --- resolution ---
        static const char *selectResolutionDolphin[] = { "Native Wii", "2x Native (720p)", "3x Native (1080p)", "4x Native (1440p)", "5x Native ", "6x Native (4K)", "7x Native ", "8x Native (5K)" };

        SCREEN_PopupMultiChoice *selectResolutionDolphinChoice = dolphinSettings->Add(new SCREEN_PopupMultiChoice(&m_InputResDolphin, 
            dol->T("Resolution"), selectResolutionDolphin, 0, ARRAY_SIZE(selectResolutionDolphin), dol->GetName(), screenManager(), new LayoutParams(FILL_PARENT,85.0f)));

        // -------- vsync --------
        CheckBox *vSyncDolphin = dolphinSettings->Add(new CheckBox(&m_InputVSyncDolphin, dol->T("Supress screen tearing", "Supress screen tearing (VSync)"),"", new LayoutParams(FILL_PARENT,85.0f)));

        // -------- PCSX2 --------

        // horizontal layout for margins
        LinearLayout *horizontalLayoutPCSX2 = new LinearLayout(ORIENT_HORIZONTAL, new LayoutParams(tabLayoutWidth, FILL_PARENT));
        m_TabHolder->AddTab(ge->T("PCSX2"), horizontalLayoutPCSX2);
        horizontalLayoutPCSX2->Add(new Spacer(tabMarginLeftRight));

        ViewGroup *PCSX2SettingsScroll = new ScrollView(ORIENT_VERTICAL, new LinearLayoutParams(tabLayoutWidth, FILL_PARENT));
        horizontalLayoutPCSX2->Add(PCSX2SettingsScroll);
        PCSX2SettingsScroll->SetTag("PCSX2Settings");
        LinearLayout *PCSX2Settings = new LinearLayout(ORIENT_VERTICAL);
        PCSX2SettingsScroll->Add(PCSX2Settings);

        // -------- general --------

        // horizontal layout for margins
        LinearLayout *horizontalLayoutGeneral = new LinearLayout(ORIENT_HORIZONTAL, new LayoutParams(tabLayoutWidth, FILL_PARENT));
        m_TabHolder->AddTab(ge->T("General"), horizontalLayoutGeneral);
        horizontalLayoutGeneral->Add(new Spacer(tabMarginLeftRight));

        ViewGroup *generalSettingsScroll = new ScrollView(ORIENT_VERTICAL, new LinearLayoutParams(tabLayoutWidth, FILL_PARENT));
        horizontalLayoutGeneral->Add(generalSettingsScroll);
        generalSettingsScroll->SetTag("GeneralSettings");
        LinearLayout *generalSettings = new LinearLayout(ORIENT_VERTICAL);
        generalSettingsScroll->Add(generalSettings);

        generalSettings->Add(new ItemHeader(ge->T("General settings for Marley")));

        // -------- toggle fullscreen --------
        CheckBox *vToggleFullscreen = generalSettings->Add(new CheckBox(&CoreSettings::m_EnableFullscreen, ge->T("Fullscreen", "Fullscreen"),"", new LayoutParams(FILL_PARENT,85.0f)));
        vToggleFullscreen->OnClick.Handle(this, &SettingsScreen::OnFullscreenToggle);

        // -------- system sounds --------
        CheckBox *vSystemSounds = generalSettings->Add(new CheckBox(&CoreSettings::m_EnableSystemSounds, ge->T("Enable system sounds", "Enable system sounds"),"", new LayoutParams(FILL_PARENT,85.0f)));
        vSystemSounds->OnClick.Add([=](EventParams &e) 
        {
            return SCREEN_UI::EVENT_CONTINUE;
        });

        // desktop volume
        Sound::GetDesktopVolume(m_GlobalVolume);
        const int VOLUME_OFF = 0;
        const int VOLUME_MAX = 100;

        SCREEN_PopupSliderChoice *volume = generalSettings->Add(new SCREEN_PopupSliderChoice(&m_GlobalVolume, VOLUME_OFF, VOLUME_MAX, ge->T("Global Volume"), "", new LayoutParams(FILL_PARENT,85.0f)));
        m_GlobalVolumeEnabled = true;
        volume->SetEnabledPtr(&m_GlobalVolumeEnabled);
        volume->SetZeroLabel(ge->T("Mute"));

        volume->OnChange.Add([=](EventParams &e) 
        {
            Sound::SetDesktopVolume(m_GlobalVolume);
            return SCREEN_UI::EVENT_CONTINUE;
        });

    //    // audio device
    //
    //    std::vector<std::string> audioDeviceList;
    //    std::vector<std::string> audioDeviceListStripped;
    //    SCREEN_PSplitString(SCREEN_System_GetProperty(SYSPROP_AUDIO_DEVICE_LIST), '\0', audioDeviceList);
    //    for (auto entry : audioDeviceList)
    //    {
    //        entry = entry.substr(0,entry.find("{"));
    //        audioDeviceListStripped.push_back(entry);
    //    }
    //    auto tmp = new SCREEN_PopupMultiChoiceDynamic(&audioDevice, ge->T("Device"), audioDeviceListStripped, nullptr, screenManager(), new LayoutParams(FILL_PARENT,f85));
    //    SCREEN_PopupMultiChoiceDynamic *audioDevice = generalSettings->Add(tmp);
    //
    //    audioDevice->OnChoice.Handle(this, &SCREEN_SettingsScreen::OnAudioDevice);

        // -------- theme --------
        static const char *uiTheme[] = 
        {
            "Retro",
            "Plain"
        };

        SCREEN_PopupMultiChoice *uiThemeChoice = generalSettings->Add(new SCREEN_PopupMultiChoice(&CoreSettings::m_UITheme, ge->T("Theme"),
            uiTheme, 0, ARRAY_SIZE(uiTheme), ge->GetName(), screenManager(), new LayoutParams(FILL_PARENT, 85.0f)));
        uiThemeChoice->OnChoice.Handle(this, &SettingsScreen::OnThemeChanged);

        // -------- credits --------

        // horizontal layout for margins
        LinearLayout *horizontalLayoutCredits = new LinearLayout(ORIENT_HORIZONTAL, new LayoutParams(tabLayoutWidth, FILL_PARENT));
        m_TabHolder->AddTab(ge->T("Credits"), horizontalLayoutCredits);
        horizontalLayoutCredits->Add(new Spacer(iconWidth));

        LinearLayout *logos = new LinearLayout(ORIENT_VERTICAL);
        horizontalLayoutCredits->Add(logos);
        ImageView* mednafenLogo = new ImageView(m_SpritesheetMarley->GetSprite(I_LOGO_MEDNAFEN), new AnchorLayoutParams(192.0f, 128.0f, 1.0f, 1.0f, NONE, NONE, false));
        ImageView* mupenLogo    = new ImageView(m_SpritesheetMarley->GetSprite(I_LOGO_MUPEN),    new AnchorLayoutParams(192.0f, 128.0f, 1.0f, 1.0f, NONE, NONE, false));
        ImageView* ppssppLogo   = new ImageView(m_SpritesheetMarley->GetSprite(I_LOGO_PPSSPP),   new AnchorLayoutParams(192.0f, 128.0f, 1.0f, 1.0f, NONE, NONE, false));
        ImageView* dolphinLogo  = new ImageView(m_SpritesheetMarley->GetSprite(I_LOGO_DOLPHIN),  new AnchorLayoutParams(192.0f, 128.0f, 1.0f, 1.0f, NONE, NONE, false));
        ImageView* pcsx2Logo    = new ImageView(m_SpritesheetMarley->GetSprite(I_LOGO_PCSX2),    new AnchorLayoutParams(192.0f, 128.0f, 1.0f, 1.0f, NONE, NONE, false));
        logos->Add(new Spacer(27));
        logos->Add(mednafenLogo);
        logos->Add(mupenLogo);
        logos->Add(ppssppLogo);
        logos->Add(dolphinLogo);
        logos->Add(pcsx2Logo);

        LinearLayout *credits = new LinearLayout(ORIENT_VERTICAL);
        horizontalLayoutCredits->Add(credits);
        credits->Add(new Spacer(iconWidth));

        credits->Add(new TextView
        (
            "\n"
            "     Mednafen:      mednafen.github.io (license: GNU GPLv2)\n"
            "\n"
            "     Mupen64Plus: mupen64plus.org (license: GNU GPL)\n"
            "\n"
            "     PPSSPP:          www.ppsspp.org (license: GNU GPLv2)\n"
            "\n"
            "     Dolphin:         dolphin-emu.org (license: GNU GPLv2)\n"
            "\n"
            "     PCSX2:            pcsx2.net (license: GNU GPLv2)\n",
            ALIGN_LEFT | ALIGN_VCENTER | FLAG_WRAP_TEXT, true, new LinearLayoutParams(availableWidth - 3 * iconWidth - 64.0f, 500.0f)));

        LOG_APP_INFO("UI: views for setting screen created");
    }

    void SettingsScreen::onFinish(DialogResult result) {}

    void SettingsScreen::update()
    {
        m_IsCreditsScreen = m_TabHolder->GetCurrentTab() == CREDITS_SCREEN;
        m_IsCintrollerSetupScreen = m_TabHolder->GetCurrentTab() == CONTROLLER_SETUP_SCREEN;

        if (m_TabHolder->HasFocus(m_LastTab))
        {
            m_TabHolder->enableAllTabs();
        }
        else
        {
            m_TabHolder->disableAllTabs();
            m_TabHolder->SetEnabled(m_LastTab);
        }

        SCREEN_UIScreen::update();
    }

    SCREEN_UI::EventReturn SettingsScreen::OnFullscreenToggle(SCREEN_UI::EventParams &e)
    {
        Engine::m_Engine->ToggleFullscreen();
        return SCREEN_UI::EVENT_DONE;
    }


    SCREEN_UI::EventReturn SettingsScreen::OnThemeChanged(SCREEN_UI::EventParams &e)
    {
        UI::m_ScreenManager->RecreateAllViews();
        return SCREEN_UI::EVENT_DONE;
    }

    SCREEN_UI::LinearLayout* SettingsScreen::BiosInfo(std::string infoText, bool biosFound)
    {
        using namespace SCREEN_UI;
        
        LinearLayout *horizontalLayoutInfoText = new LinearLayout(ORIENT_HORIZONTAL, new LayoutParams(FILL_PARENT, FILL_PARENT));
        horizontalLayoutInfoText->Add(new Spacer(162.0f, 0.0f));
        
        std::string status = (biosFound ? ": found" : ": not found");
        
        int height = ( (CoreSettings::m_UITheme == THEME_RETRO) ? WRAP_CONTENT : 40.0f);
        TextView* biosFoundInfo = new TextView(infoText + status, ALIGN_CENTER, true, new LinearLayoutParams(1435.0f, height, 1.0f));
        if (CoreSettings::m_UITheme == THEME_RETRO) biosFoundInfo->SetShadow(true);
        
        horizontalLayoutInfoText->Add(biosFoundInfo);

        return horizontalLayoutInfoText;
    }
}
