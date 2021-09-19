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
#include <filesystem>

#include "marley/emulation/ROM.h"
#include "file.h"

namespace MarleyApp
{
    void ROM::FindAllFiles(const std::string& directory, std::list<std::string>* tmpList, std::list<std::string>* toBeRemoved, bool recursiveSearch)
    {

        int i = 0;

        for (const auto& fileIterator : std::filesystem::directory_iterator(directory))
        {
            std::filesystem::path path{std::filesystem::path(fileIterator)};
            std::string filenameWithPath    = path.string();
            std::string filenameWithoutPath = GetFilenameWithoutPath(path);

            if (IsDirectory(filenameWithPath))
            {
                if (recursiveSearch)
                {
                    if ((filenameWithoutPath != ".") && (filenameWithoutPath != ".."))
                    {
                        FindAllFiles(filenameWithPath, tmpList, toBeRemoved);
                    }
                }
            }
            else
            {
                std::string ext = GetExtension(path);
                ext = ext.substr(ext.find_last_of(".") + 1);

                std::transform(ext.begin(), ext.end(), ext.begin(),
                    [](unsigned char c){ return std::tolower(c); });

                std::string filenameWithPathLowerCase = filenameWithPath;
                std::transform(filenameWithPathLowerCase.begin(), filenameWithPathLowerCase.end(), filenameWithPathLowerCase.begin(),
                    [](unsigned char c){ return std::tolower(c); });

                for (int i=0;i<m_FileTypes.size();i++)
                {
                    if ((ext == m_FileTypes[i])  && \
                        (filenameWithPathLowerCase.find("battlenet") ==  std::string::npos) &&\
                        (filenameWithPathLowerCase.find("ps3") ==  std::string::npos) &&\
                        (filenameWithPathLowerCase.find("ps4") ==  std::string::npos) &&\
                        (filenameWithPathLowerCase.find("xbox") ==  std::string::npos) &&\
                        (filenameWithPathLowerCase.find("bios") ==  std::string::npos) &&\
                        (filenameWithPathLowerCase.find("firmware") ==  std::string::npos))
                    {
                        if (ext == "mdf")
                        {
                            std::string bin_file;
                            bin_file = filenameWithPath.substr(0,filenameWithPath.find_last_of(".")) + ".bin";
                            if (!FileExists(bin_file)) tmpList[0].push_back(filenameWithPath);
                        }
                        else if (ext == "cue")
                        {
                            if(CheckForCueFiles(filenameWithPath, toBeRemoved))
                              tmpList[0].push_back(filenameWithPath);
                        }
                        else
                        {
                            tmpList[0].push_back(filenameWithPath);
                        }
                    }
                }
            }
        }
    }

    void ROM::StripList(std::list<std::string> *tmpList,std::list<std::string> *toBeRemoved)
    {
        std::list<std::string>::iterator iteratorTmpList;
        std::list<std::string>::iterator iteratorToBeRemoved;

        std::string strRemove, strRemoveNoPath, strList, strListNoPath;
        int i,j;

        iteratorToBeRemoved = toBeRemoved[0].begin();

        for (i=0; i < toBeRemoved[0].size(); i++)
        {
            strRemove = *iteratorToBeRemoved;
            iteratorToBeRemoved++;
            iteratorTmpList = tmpList[0].begin();

            strRemoveNoPath = strRemove;
            if(strRemoveNoPath.find("/") != std::string::npos)
            {
                strRemoveNoPath = strRemove.substr(strRemoveNoPath.find_last_of("/") + 1);
            }

            for (j=0; j<tmpList[0].size(); j++)
            {
                strList = *iteratorTmpList;

                strListNoPath = strList;
                if(strListNoPath.find("/") != std::string::npos)
                {
                    strListNoPath = strListNoPath.substr(strListNoPath.find_last_of("/") + 1);
                }

                if ( strRemoveNoPath == strListNoPath )
                {
                    tmpList[0].erase(iteratorTmpList++);
                }
                else
                {
                    iteratorTmpList++;
                }
            }
        }
    }

    bool ROM::CheckForCueFiles(const std::string& filenameWithPath, std::list<std::string> *toBeRemoved)
    {
        std::string line, name;

        std::ifstream cueFile(filenameWithPath.c_str());
        if (!cueFile.is_open())
        {
            LOG_APP_WARN("Could not open cue file: {0}",filenameWithPath);
            return false;
        }
        
        while ( getline(cueFile, line))
        {
            if (line.find("FILE") != std::string::npos)
            {
                filenameWithPath.substr(filenameWithPath.find_last_of(".") + 1);

                int start  = line.find("\"")+1;
                int length = line.find_last_of("\"")-start;
                name = line.substr(start,length);

                std::string nameWithPath = name;
                if (filenameWithPath.find("/") != std::string::npos)
                {
                    nameWithPath = filenameWithPath.substr(0,filenameWithPath.find_last_of("/")+1) + name;
                }

                if (FileExists(name) || (FileExists(nameWithPath)))
                {
                    toBeRemoved[0].push_back(name);
                    return true;
                } 
                else 
                {
                    return false;
                }
            }
        }

        return false;
    }

    bool ROM::FindInVector(std::vector<std::string>* vec, const std::string& str)
    {
        
        bool ok = false;
        std::string element;

        std::vector<std::string>::iterator it;

        for(it=vec[0].begin(); it < vec[0].end(); it++)
        {
            element = *it;
            
            int constexpr EQUAL = 0; 
            if (element.compare(str) == EQUAL)
            {
                ok = true;
                break;
            }
            
        }
        return ok;
    }

    void ROM::FinalizeList(std::list<std::string>* tmpList)
    {
        std::list<std::string>::iterator iteratorTmpList;
        std::string strList;

        iteratorTmpList = tmpList[0].begin();

        for (int i=0; i < tmpList[0].size(); i++)
        {
            strList = *iteratorTmpList;
            if (!FindInVector(&m_Games, strList)) //avoid duplicates
            {
                m_Games.push_back(strList);
            }
            iteratorTmpList++;
        }
    }
}
