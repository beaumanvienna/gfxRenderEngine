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

#include "soundDevice.h"
#include "system.h"

std::vector<std::string>& SoundDevice::GetSoundDeviceList()
{
    RefreshSoundDeviceList();
    return m_SoundDeviceList;
}

void PactlOutput::ParseLine(const std::string& line)
{
    if (!line.length())
    {
        return;
    }

    if (line.find("available: no") != std::string::npos)
    {
        return;
    }

    if (line.find("index:") != std::string::npos)
    {
        if (m_State == OFF)
        {
            m_State = INDEX;
            std::string index = line.substr(line.find(":")+1);
            m_Index = std::stoi(index);
            m_Categories.clear();
        }
    }

    if (line.find("device.description = \"") != std::string::npos)
    {
        if (m_State == INDEX)
        {
            m_State = DESCRIPTION;
            m_Description = line.substr(line.find("\"")+1);
            m_Description = m_Description.substr(0, ((m_Description.length()-1) > 15 ? 14 : m_Description.length()-1));
        }
    }

    if (line.find("profiles:") != std::string::npos)
    {
        if (m_State == DESCRIPTION)
        {
            m_State = PROFILES;
        }
    }

    if (line.find("active profile:") != std::string::npos)
    {
        m_State = OFF;
    }

    if ( (line.find("output:") != std::string::npos) && (m_State == PROFILES) )
    {
        std::string output = line.substr(line.find(":")+1);
        std::string category = output.substr(0, output.find("-"));
        if (FindProfile(category))
        {
            return;
        }
        m_Categories.push_back(category);
        
        std::string profile = output.substr(0, output.find(":"));
        m_Profiles.push_back( { m_Index, m_Description, std::string("output:") + profile});
        
        profile = profile.substr(0, (profile.length()>14?14:profile.length()));
        std::string entry = m_Description + std::string{","} + profile;
        m_Entries.push_back(entry);
    }
}

bool PactlOutput::FindProfile(const std::string& category)
{
    for (auto entry : m_Categories)
    {
        if (entry == category) return true;
    }
    return false;
}

void SoundDevice::Print()
{
    for (auto entry : m_SoundDeviceList)
    {
        LOG_CORE_CRITICAL("entry: {0}", entry);
    }
    for (auto profile : m_SoundDeviceProfiles)
    {
        LOG_CORE_CRITICAL("index: {0}, description: {1}, profile: {2}", profile.m_Index, profile.m_Description, profile.m_Profile);
    }
}

void SoundDevice::ActivateDeviceProfile(const std::string& profile)
{
    uint i = 0;
    for (auto entry : m_SoundDeviceList)
    {
        if (entry == profile)
        {
            std::string cmd = "pactl set-card-profile " + std::to_string(m_SoundDeviceProfiles[i].m_Index) + std::string(" ") + m_SoundDeviceProfiles[i].m_Profile;
            EngineCore::Exec(cmd);
            cmd = "pacmd set-default-sink " + std::to_string(m_SoundDeviceProfiles[i].m_Index);
            EngineCore::Exec(cmd);
            break;
        }
        i++;
    }
}

void SoundDevice::RefreshSoundDeviceList()
{
    m_SoundDeviceList.clear();

    std::istringstream strStream(EngineCore::Exec("pacmd list-cards"));
    std::string line;
    while (std::getline(strStream, line))
    {
        m_PactlOutput.ParseLine(line);
    }
}
