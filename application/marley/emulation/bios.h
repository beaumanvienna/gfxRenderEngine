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

#include <list>
#include <iostream>

#include "engine.h"

namespace MarleyApp
{

    class Bios
    {
    public:

        enum
        {
            PS1_BIOS_SIZE           = 524288,
            SEGA_SATURN_BIOS_SIZE   = 524288,
            PS2_BIOS_SIZE           = 4194304,
            SCPH5500_BIN            = 21715608,
            SCPH5501_BIN            = 22714036,
            SCPH5502_BIN            = 24215776,
            SCPH77000_BIN           = 56837872,
            SCPH77001_BIN           = 162002608,
            SCPH77002_BIN           = 169569744,
            SEGA_SATURN_BIOS_JP     = 19759492,
            SEGA_SATURN_BIOS_NA_EU  = 19688652,

            BIOS_NA                 = 10,
            BIOS_JP                 = 11,
            BIOS_EU                 = 12
        };


    public:

        Bios(const std::string& filename = "");

        void SetSearchPath(const std::string& filename);
        void SetBaseDirectory();
        void InstallAllFiles();
        void CheckBiosFiles();
        void QuickCheckBiosFiles();

        bool m_BiosFoundPS1;
        bool m_BiosFoundPS1Japan;
        bool m_BiosFoundPS1NorthAmerica;
        bool m_BiosFoundPS1Europe;

        bool m_BiosFoundPS2;
        bool m_BiosFoundPS2Japan;
        bool m_BiosFoundPS2NorthAmerica;
        bool m_BiosFoundPS2Europe;
        
        bool m_BiosFoundSegaSaturn;
        bool m_BiosFoundSegaSaturnJapan;
        bool m_BiosFoundSegaSaturnNorthAmericaEurope;

    private:

        void CheckBiosPSX(void);
        void QuickCheckBiosPSX(void);
        void FindBiosPSX(void);
        void CheckBiosSegaSaturn(void);
        void QuickCheckBiosSegaSaturn(void);
        void FindBiosSegaSaturn(void);
        uint64 CalcChecksum(const std::string& filename);
        bool CheckBiosExists(const std::string& filename, int checksum);
        void FindAllFiles(const std::string& directory, std::list<std::string> *tmpListPS1, std::list<std::string> *tmpListPS2 = nullptr);

        std::string m_SearchPathBios;
        std::string m_BaseDirectory;

        std::string m_BiosPathPS1;
        std::string m_FilenameBiosPS1Japan;
        std::string m_FilenameBiosPS1NorthAmerica;
        std::string m_FilenameBiosPS1Europe;

        std::string m_BiosPathPS2;
        std::string m_FilenameBiosPS2Japan;
        std::string m_FilenameBiosPS2NorthAmerica;
        std::string m_FilenameBiosPS2Europe;
        
        std::string m_FilenameBiosSegaSaturnJapan;
        std::string m_FilenameBiosSegaSaturnNorthAmericaEurope;

    };
}
