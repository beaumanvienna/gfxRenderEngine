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
   
#include <iostream>

#include "engine.h"
#include "core.h"
#include "appSettings.h"

std::string AppSettings::m_LastGamePath;
std::string AppSettings::m_UITheme;
std::string AppSettings::m_SearchDirGames;

void AppSettings::InitDefaults()
{
    m_LastGamePath   = Engine::m_Engine->GetHomeDirectory();
    m_SearchDirGames = Engine::m_Engine->GetHomeDirectory();
    m_UITheme        = "Retro";
}

void AppSettings::RegisterSettings()
{
    m_SettingsManager->PushSetting<std::string> ("LastGamePath",   &m_LastGamePath);
    m_SettingsManager->PushSetting<std::string> ("SearchDirGames", &m_SearchDirGames);
    m_SettingsManager->PushSetting<std::string> ("UITheme",        &m_UITheme);
}

void AppSettings::PrintSettings() const
{
    LOG_APP_INFO("AppSettings: key '{0}', value is {1}", "LastGamePath",   m_LastGamePath);
    LOG_APP_INFO("AppSettings: key '{0}', value is {1}", "SearchDirGames", m_SearchDirGames);
    LOG_APP_INFO("AppSettings: key '{0}', value is {1}", "UITheme",        m_UITheme);
}
