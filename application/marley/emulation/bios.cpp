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

#include <list>
#include <fstream>

#include "marley/emulation/bios.h"
#include "marley/marley.h"
#include "file.h"

namespace MarleyApp
{
    Bios::Bios(const std::string& filename)
        : m_BiosFoundPS1(false),
          m_BiosFoundPS1Japan(false),
          m_BiosFoundPS1NorthAmerica(false),
          m_BiosFoundPS1Europe(false),
          m_BiosFoundPS2(false),
          m_BiosFoundPS2Japan(false),
          m_BiosFoundPS2NorthAmerica(false),
          m_BiosFoundPS2Europe(false)
    {
        if (filename != "") SetSearchPath(filename);
        m_BaseDirectory = Marley::m_EmulationUtils->GetConfigFolder();

        // PS1
        m_FilenameBiosPS1Japan = m_BaseDirectory + "scph5500.bin";
        m_FilenameBiosPS1NorthAmerica = m_BaseDirectory + "scph5501.bin";
        m_FilenameBiosPS1Europe = m_BaseDirectory + "scph5502.bin";

        // PS2
        m_FilenameBiosPS2Japan = m_BaseDirectory + "scph77000.bin";
        m_FilenameBiosPS2NorthAmerica = m_BaseDirectory + "scph77001.bin";
        m_FilenameBiosPS2Europe = m_BaseDirectory + "scph77002.bin";

        // Sega Saturn
        m_FilenameBiosSegaSaturnJapan = m_BaseDirectory + "mednafen/firmware/sega_101.bin";
        m_FilenameBiosSegaSaturnNorthAmericaEurope = m_BaseDirectory + "mednafen/firmware/mpr-17933.bin";
    }

    void Bios::SetSearchPath(const std::string& filename)
    {
        if (IsDirectory(filename))
        {
            m_SearchPathBios = filename;
        }
        else
        {
            LOG_APP_WARN("{0} is not a directory, couldn't set bios search path", filename);
        }
    }
    
    void Bios::CheckBiosFiles()
    {
        CheckBiosPSX();
        CheckBiosSegaSaturn();
    }

    void Bios::QuickCheckBiosFiles()
    {
        QuickCheckBiosPSX();
        QuickCheckBiosSegaSaturn();
    }
    
    void Bios::CheckBiosPSX(void)
    {
        // ---------- PS1 ----------

        // check if PS1 files are already installed in base directory
        if (CheckBiosExists(m_FilenameBiosPS1Japan, SCPH5500_BIN))
        {
            LOG_APP_INFO("PS1 bios found with signature 'Japan SCPH-5500/v3.0J'        : {0}", m_FilenameBiosPS1Japan);
            m_BiosFoundPS1Japan = true;
        }

        if (CheckBiosExists(m_FilenameBiosPS1NorthAmerica, SCPH5501_BIN))
        {
            LOG_APP_INFO("PS1 bios found with signature 'North America SCPH-5501/v3.0A': {0}", m_FilenameBiosPS1NorthAmerica);
            m_BiosFoundPS1NorthAmerica = true;
        }

        if (CheckBiosExists(m_FilenameBiosPS1Europe, SCPH5502_BIN))
        {
            LOG_APP_INFO("PS1 bios found with signature 'Europe SCPH-5502/v3.0E'       : {0}", m_FilenameBiosPS1Europe);
            m_BiosFoundPS1Europe = true;
        }

        // ---------- PS2 ----------

        // check if PS2 files are already installed in the base directory
        if (CheckBiosExists(m_FilenameBiosPS2Japan, SCPH77000_BIN))
        {
            LOG_APP_INFO("PS2 bios found with signature 'Japan SCPH-77000'        : {0}", m_FilenameBiosPS2Japan);
            m_BiosFoundPS2Japan = true;
        }

        if (CheckBiosExists(m_FilenameBiosPS2NorthAmerica, SCPH77001_BIN))
        {
            LOG_APP_INFO("PS2 bios found with signature 'North America SCPH-77001': {0}", m_FilenameBiosPS2NorthAmerica);
            m_BiosFoundPS2NorthAmerica = true;
        }

        if (CheckBiosExists(m_FilenameBiosPS2Europe, SCPH77002_BIN))
        {
            LOG_APP_INFO("PS2 bios found with signature 'Europe SCPH-77002'       : {0}", m_FilenameBiosPS2Europe);
            m_BiosFoundPS2Europe = true;
        }
    }
    
    
    
    void Bios::QuickCheckBiosPSX(void)
    {
        // ---------- PS1 ----------

        // check if PS1 files are already installed in base directory
        if (!FileExists(m_FilenameBiosPS1Japan))
        {
            LOG_APP_INFO("Hint: You may want to install a PS1 bios file with signature 'Japan SCPH-5500/v3.0J'");
        }

        if (!FileExists(m_FilenameBiosPS1NorthAmerica))
        {
            LOG_APP_INFO("Hint: You may want to install a PS1 bios file with signature 'North America SCPH-5501/v3.0A': {0}");
        }

        if (!FileExists(m_FilenameBiosPS1Europe))
        {
            LOG_APP_INFO("Hint: You may want to install a PS1 bios file with signature 'North America SCPH-5501/v3.0A'");
        }

        // ---------- PS2 ----------

        // check if PS2 files are already installed in the base directory
        if (!FileExists(m_FilenameBiosPS2Japan))
        {
            LOG_APP_INFO("Hint: You may want to install a PS1 bios file with signature 'Japan SCPH-77000'");
        }

        if (!FileExists(m_FilenameBiosPS2NorthAmerica))
        {
            LOG_APP_INFO("Hint: You may want to install a PS1 bios file with signature 'North America SCPH-77001'");
        }

        if (!FileExists(m_FilenameBiosPS2Europe))
        {
            LOG_APP_INFO("Hint: You may want to install a PS1 bios file with signature 'Europe SCPH-77002'");
        }
    }

    void Bios::FindBiosPSX(void)
    {
        m_BiosFoundPS1Japan         = false;
        m_BiosFoundPS1NorthAmerica  = false;
        m_BiosFoundPS1Europe        = false;
        
        m_BiosFoundPS2Japan         = false;
        m_BiosFoundPS2NorthAmerica  = false;
        m_BiosFoundPS2Europe        = false;
        
        CheckBiosPSX();
        
        // if not all files are installed in base directory search bios path
        if (!(m_BiosFoundPS1Japan && m_BiosFoundPS1NorthAmerica && m_BiosFoundPS1Europe && m_BiosFoundPS2Japan && m_BiosFoundPS2NorthAmerica && m_BiosFoundPS2Europe))
        {
            if (m_SearchPathBios != "")
            {
                std::list<std::string> tmpListPS1;
                std::list<std::string> tmpListPS2;

                FindAllFiles(m_SearchPathBios, &tmpListPS1, &tmpListPS2);

                for (std::string str : tmpListPS1)
                {

                    if (CheckBiosExists(str, SCPH5500_BIN) && !m_BiosFoundPS1Japan)
                    {
                        LOG_APP_INFO("PS1 bios found with signature 'Japan SCPH-5500/v3.0J'        : {0}", str);
                        m_BiosFoundPS1Japan = CopyFile(str, m_FilenameBiosPS1Japan);
                    }
                    if (CheckBiosExists(str, SCPH5501_BIN) && !m_BiosFoundPS1NorthAmerica)
                    {
                        LOG_APP_INFO("PS1 bios found with signature 'North America SCPH-5501/v3.0A': {0}", str);
                        m_BiosFoundPS1NorthAmerica = CopyFile(str, m_FilenameBiosPS1NorthAmerica);
                    }
                    if (CheckBiosExists(str, SCPH5502_BIN) && !m_BiosFoundPS1Europe)
                    {
                        LOG_APP_INFO("PS1 bios found with signature 'Europe SCPH-5502/v3.0E'       : {0}", str);
                        m_BiosFoundPS1Europe = CopyFile(str, m_FilenameBiosPS1Europe);
                    }
                }

                for (std::string str : tmpListPS2)
                {
                    if (CheckBiosExists(str, SCPH77000_BIN) && !m_BiosFoundPS2Japan)
                    {
                        LOG_APP_INFO("PS2 bios found with signature 'Japan SCPH-77000'        : {0}", str);
                        m_BiosFoundPS2Japan = CopyFile(str, m_FilenameBiosPS2Japan);
                    }
                    if (CheckBiosExists(str, SCPH77001_BIN) && !m_BiosFoundPS2NorthAmerica)
                    {
                        LOG_APP_INFO("PS2 bios found with signature 'North America SCPH-77001': {0}", str);
                        m_BiosFoundPS2NorthAmerica = CopyFile(str, m_FilenameBiosPS2NorthAmerica);
                    }
                    if (CheckBiosExists(str, SCPH77002_BIN) && !m_BiosFoundPS2Europe)
                    {
                        LOG_APP_INFO("PS2 bios found with signature 'Europe SCPH-77002'       : {0}", str);
                        m_BiosFoundPS2Europe = CopyFile(str, m_FilenameBiosPS2Europe);
                    }
                }
            }
        }

        m_BiosFoundPS1 = m_BiosFoundPS1Japan || m_BiosFoundPS1NorthAmerica || m_BiosFoundPS1Europe;
        m_BiosFoundPS2 = m_BiosFoundPS2Japan || m_BiosFoundPS2NorthAmerica || m_BiosFoundPS2Europe;

        if (m_BiosFoundPS2)
        {
            if (m_BiosFoundPS2NorthAmerica)
            {
                m_BiosPathPS2 = m_FilenameBiosPS2NorthAmerica;
            }
            else if (m_BiosFoundPS2Japan)
            {
                m_BiosPathPS2 = m_FilenameBiosPS2Japan;
            }
            else if (m_BiosFoundPS2Europe)
            {
                m_BiosPathPS2 = m_FilenameBiosPS2Europe;
            }
        }

        std::string GSdxConfigFile = m_BaseDirectory + "PCSX2/inis/GSdx.ini";
        std::string line, str_dec;
        std::string::size_type sz;

        // default
        m_BiosPathPS2 = m_BaseDirectory + "scph77001.bin";
        
        //if PCSX2 config exists get actual value from there
        std::ifstream GSdxConfigFilehandle(GSdxConfigFile);
        if (GSdxConfigFilehandle.is_open())
        {
            while ( getline (GSdxConfigFilehandle,line))
            {
                if(line.find("bios_region") != std::string::npos)
                {
                    str_dec = line.substr(line.find_last_of("=") + 1);
                    int biosRegion = std::stoi(str_dec,&sz);
                    if ((biosRegion < BIOS_NA) || (biosRegion > BIOS_EU))
                    {
                        biosRegion = BIOS_NA;
                    }

                    if ((biosRegion == BIOS_NA) && m_BiosFoundPS2NorthAmerica )
                    {
                        m_BiosPathPS2 = m_BaseDirectory + "scph77001.bin";
                    }
                    else if ((biosRegion == BIOS_JP) && m_BiosFoundPS2Japan )
                    {
                        m_BiosPathPS2 = m_BaseDirectory + "scph77000.bin";
                    }
                    else if ((biosRegion == BIOS_EU) && m_BiosFoundPS2Europe )
                    {
                        m_BiosPathPS2 = m_BaseDirectory + "scph77002.bin";
                    }

                    break;
                }
            }
        }
    }

    void Bios::CheckBiosSegaSaturn(void)
    {
        // check if sega saturn bios files are already installed in base directory
        if (CheckBiosExists(m_FilenameBiosSegaSaturnJapan, SEGA_SATURN_BIOS_JP))
        {
            LOG_APP_INFO("Sega Saturn bios found with signature 'Japan'                 : {0}", m_FilenameBiosSegaSaturnJapan);
            m_BiosFoundSegaSaturnJapan = true;
        }

        if (CheckBiosExists(m_FilenameBiosSegaSaturnNorthAmericaEurope, SEGA_SATURN_BIOS_NA_EU))
        {
            LOG_APP_INFO("Sega Saturn bios found with signature 'North America / Europa': {0}", m_FilenameBiosSegaSaturnNorthAmericaEurope);
            m_BiosFoundSegaSaturnNorthAmericaEurope = true;
        }
    }

    void Bios::QuickCheckBiosSegaSaturn(void)
    {
        // check if sega saturn bios files are already installed in base directory
        if (!FileExists(m_FilenameBiosSegaSaturnJapan))
        {
            LOG_APP_INFO("Hint: You may want to install a PS1 bios file with signature 'Japan'");
        }

        if (!FileExists(m_FilenameBiosSegaSaturnNorthAmericaEurope))
        {
            LOG_APP_INFO("Hint: You may want to install a PS1 bios file with signature 'North America / Europa'");
        }
    }

    void Bios::FindBiosSegaSaturn(void)
    {
        m_BiosFoundSegaSaturnJapan = false;
        m_BiosFoundSegaSaturnNorthAmericaEurope = false;
        
        std::string targetDirectory = m_BaseDirectory + "mednafen/firmware";
        if (!IsDirectory(targetDirectory))
        {
            CreateDirectory(m_BaseDirectory + "mednafen/firmware");
        }
        else
        {
            CheckBiosSegaSaturn();
        }

        // if not all files are installed in base directory search bios path
        if (!(m_BiosFoundSegaSaturnJapan && m_BiosFoundSegaSaturnNorthAmericaEurope))
        {
            if (m_SearchPathBios != "")
            {
                std::list<std::string> tmpListSegaSaturn;

                FindAllFiles(m_SearchPathBios, &tmpListSegaSaturn);

                for (std::string str : tmpListSegaSaturn)
                {
                    if (CheckBiosExists(str, SEGA_SATURN_BIOS_JP) && !m_BiosFoundSegaSaturnJapan)
                    {
                        LOG_APP_INFO("Sega Saturn bios found with signature 'Japan'                 : {0}", str);
                        m_BiosFoundSegaSaturnJapan = CopyFile(str, m_FilenameBiosSegaSaturnJapan);
                    }
                    if (CheckBiosExists(str, SEGA_SATURN_BIOS_NA_EU) && !m_BiosFoundSegaSaturnNorthAmericaEurope)
                    {
                        LOG_APP_INFO("Sega Saturn bios found with signature 'North America / Europa': {0}", str);
                        m_BiosFoundSegaSaturnNorthAmericaEurope = CopyFile(str, m_FilenameBiosSegaSaturnNorthAmericaEurope);
                    }
                }
            }
        }

        m_BiosFoundSegaSaturn = m_BiosFoundSegaSaturnJapan || m_BiosFoundSegaSaturnNorthAmericaEurope;
    }

    uint64 Bios::CalcChecksum(const std::string& filename)
    {
        std::ifstream ifs(filename.c_str(), std::ios::binary | std::ios::ate);
        std::ifstream::pos_type pos = ifs.tellg();
        uint64 checksum = 0;

        int length = pos;

        char *pChars = new char[length];
        ifs.seekg(0, std::ios::beg);
        ifs.read(pChars, length);

        for(int i = 0; i < length; i++)
            checksum += pChars[i] + (pChars[i] ^ 0x55);

        delete pChars;

        return checksum;
    }
    
    bool Bios::CheckBiosExists(const std::string& filename, int checksum)
    {
        if (!FileExists(filename)) return false;
        return CalcChecksum(filename) == checksum;
    }

    void Bios::FindAllFiles(const std::string& directory, std::list<std::string> *tmpListPS1, std::list<std::string> *tmpListPS2)
    {
        for (const auto& fileIterator : std::filesystem::directory_iterator(directory))
        {
            std::filesystem::path path{std::filesystem::path(fileIterator)};
            std::string filenameWithPath    = path.string();
            std::string filenameWithoutPath = GetFilenameWithoutPath(path);

            if (IsDirectory(filenameWithPath))
            {
                if ((filenameWithoutPath != ".") && (filenameWithoutPath != ".."))
                {
                    filenameWithPath +="/";
                    FindAllFiles(filenameWithPath, tmpListPS1, tmpListPS2);
                }
            }
            else
            {
                std::string ext = filenameWithPath.substr(filenameWithPath.find_last_of(".") + 1);

                std::transform(ext.begin(), ext.end(), ext.begin(),
                    [](unsigned char c){ return std::tolower(c); });

                std::string filenameWithPathLowerCase=filenameWithPath;
                std::transform(filenameWithPathLowerCase.begin(), filenameWithPathLowerCase.end(), filenameWithPathLowerCase.begin(),
                    [](unsigned char c){ return std::tolower(c); });

                if ((filenameWithPathLowerCase.find("battlenet") ==  std::string::npos) &&\
                    (filenameWithPathLowerCase.find("ps3") ==  std::string::npos) &&\
                    (filenameWithPathLowerCase.find("ps4") ==  std::string::npos) &&\
                    (filenameWithPathLowerCase.find("xbox") ==  std::string::npos))
                {
                    int fileSize = FileSize(filenameWithPath);

                    if ((fileSize == PS1_BIOS_SIZE)||(fileSize == SEGA_SATURN_BIOS_SIZE))
                    {
                        tmpListPS1[0].push_back(filenameWithPath);
                    }
                    else if ( (fileSize == PS2_BIOS_SIZE) && tmpListPS2)
                    {
                        tmpListPS2[0].push_back(filenameWithPath);
                    }
                }
            }
        }
    }

    void Bios::InstallAllFiles()
    {
        if (IsDirectory(m_SearchPathBios))
        {
            FindBiosPSX();
            FindBiosSegaSaturn();
        }
        else
        {
            LOG_APP_WARN("Search path '{0}' not found", m_SearchPathBios);
        }
    }
}
