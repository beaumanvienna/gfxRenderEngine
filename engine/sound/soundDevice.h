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

#pragma once

#include <iostream>
#include <vector>

#include "engine.h"

struct SoundCardProfile
{
    uint m_Index;
    std::string m_Description;
    std::string m_Profile;
};

class PactlOutput
{
    enum State
    {
        OFF,
        INDEX,
        DESCRIPTION,
        PROFILES,
    };
public:

    PactlOutput(std::vector<std::string>& entries, std::vector<SoundCardProfile>& profiles) 
        : m_Entries(entries), m_Profiles(profiles) { m_State = OFF; }
    void ParseLine(const std::string& line);
    std::string FindEntry(const std::string& entry);

private:

    bool FindProfile(const std::string& category);

private:

    State m_State;
    uint m_Index;
    std::string m_Description;
    std::vector<std::string>& m_Entries;
    std::vector<SoundCardProfile>& m_Profiles;

    // for each card, get only one per category (analog, hdmi, etc.)
    std::vector<std::string> m_Categories;

};


class SoundDevice
{

public:
    
    std::vector<std::string>& GetSoundDeviceList();
    void ActivateDeviceProfile(const std::string& profile);
    void Print();

private:

    void RefreshSoundDeviceList();

    std::vector<std::string> m_SoundDeviceList;
    std::vector<SoundCardProfile> m_SoundDeviceProfiles;
    PactlOutput m_PactlOutput{m_SoundDeviceList, m_SoundDeviceProfiles};

};
