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
#include <stdio.h>

#include "sound.h"

bool Sound::GetDesktopVolume(int& desktopVolume)
{
    bool ok = true;
    desktopVolume = 0;
#ifdef LINUX
    std::string command = "pactl list sinks "; 
    std::string data;
    FILE* stream;
    const int MAX_BUFFER = 8192;
    char buffer[MAX_BUFFER];

    stream = popen(command.c_str(), "r");
    if (stream)
    {
        while (!feof(stream))
        {
            if (fgets(buffer, MAX_BUFFER, stream) != nullptr)
            {
                data.append(buffer);
            }
        }
        pclose(stream);
        size_t position = data.find("Volume");
        if (position != std::string::npos)
        {
            data = data.substr(position, 50);
            position = data.find("%");
            if (position != std::string::npos)
            {
                data = data.substr(position-3, 3);
                int volume;
                ok = true;
                try
                {
                  volume = stoi(data);
                }
                catch(...)
                {
                  // if no conversion could be performed
                  ok = false;
                }
                if (ok)
                {
                    desktopVolume = volume;
                }
            }
        }
    }
#endif
    return ok;
}
