/* Copyright (c) 2013-2020 PPSSPP project
   https://github.com/hrydgard/ppsspp/blob/master/LICENSE.TXT
   
   Engine Copyright (c) 2021 Engine Development Team 
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


#include <strings.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#include <cstring>
#include <string>
#include <set>
#include <algorithm>
#include <cstdio>
#include <sys/stat.h>
#include <ctype.h>

#include "utf8.h"
#include "stringUtils.h"
#include "dirListing.h"

// Returns true if filename is a directory
bool isDirectory(const std::string & filename)
{
    FileInfo info;
    getFileInfo(filename.c_str(), &info);
    return info.isDirectory;
}

bool getFileInfo(const char *path, FileInfo * fileInfo)
{

    fileInfo->fullName = path;

    std::string copy(path);

    struct stat64 file_info;
    int result = stat64(copy.c_str(), &file_info);

    if (result < 0)
    {
        fileInfo->exists = false;
        return false;
    }

    fileInfo->isDirectory = S_ISDIR(file_info.st_mode);
    fileInfo->isWritable = false;
    fileInfo->size = file_info.st_size;
    fileInfo->exists = true;
    
    if (file_info.st_mode & 0200)
    {
        fileInfo->isWritable = true;
    }

    return true;
}

std::string getFileExtension(const std::string & fn)
{
    int pos = (int)fn.rfind(".");
    if (pos < 0) return "";
    std::string ext = fn.substr(pos + 1);
    for (size_t i = 0; i < ext.size(); i++)
    {
        ext[i] = tolower(ext[i]);
    }
    return ext;
}

bool FileInfo::operator <(const FileInfo & other) const
{
    if (isDirectory && !other.isDirectory)
    {
        return true;
    }
    else if (!isDirectory && other.isDirectory)
    {
        return false;
    }
    
    if (strcasecmp(name.c_str(), other.name.c_str()) < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

size_t getFilesInDir(const char *directory, std::vector<FileInfo> * files, const char *filter, int flags)
{
    size_t foundEntries = 0;
    std::set<std::string> filters;
    if (filter)
    {
        std::string tmp;
        while (*filter)
        {
            if (*filter == ':')
            {
                filters.insert(std::move(tmp));
            }
            else
            {
                tmp.push_back(*filter);
            }
            filter++;
        }
        if (!tmp.empty())
        {
            filters.insert(std::move(tmp));
        }
    }
#ifdef WINDOWS
    // Find the first file in the directory.
    WIN32_FIND_DATA ffd;
    HANDLE hFind = FindFirstFileEx((ConvertUTF8ToWString(directory) + L"\\*").c_str(), FindExInfoStandard, &ffd, FindExSearchNameMatch, NULL, 0);
    if (hFind == INVALID_HANDLE_VALUE) {
        return 0;
    }
    // windows loop
    do
    {
        const std::string virtualName = ConvertWStringToUTF8(ffd.cFileName);
#else
    struct dirent *result = NULL;

    //std::string directoryWithSlash = directory;
    //if (directoryWithSlash.back() != '/')
    //    directoryWithSlash += "/";

    DIR *dirp = opendir(directory);
    if (!dirp)
        return 0;
    // non windows loop
    while ((result = readdir(dirp)))
    {
        const std::string virtualName(result->d_name);
#endif
        // check for "." and ".."
        if (virtualName == "." || virtualName == "..")
            continue;

        // Remove dotfiles (optional with flag.)
        if (!(flags & GETFILES_GETHIDDEN))
        {
#ifdef WINDOWS
            if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0)
                continue;
#else
            if (virtualName[0] == '.')
                continue;
#endif
        }

        FileInfo info;
        info.name = virtualName;
        std::string dir = directory;

        size_t lastSlash = dir.find_last_of("/");
        if (lastSlash != (dir.length() - 1))
        {
            dir.append("/");
        }

        info.fullName = dir + virtualName;
        info.isDirectory = isDirectory(info.fullName);
        info.exists = true;
        info.size = 0;
        info.isWritable = false;  
        if (!info.isDirectory)
        {
            std::string ext = getFileExtension(info.fullName);
            if (filter)
            {
                if (filters.find(ext) == filters.end())
                {
                    continue;
                }
            }
        }

        if (files)
        {
            files->push_back(std::move(info));
        }
        foundEntries++;
#ifdef WINDOWS
    } while (FindNextFile(hFind, &ffd) != 0);
    FindClose(hFind);
#else
    }
    closedir(dirp);
#endif
    if (files)
    {
        std::sort(files->begin(), files->end());
    }
    return foundEntries;
}

int64_t getDirectoryRecursiveSize(const std::string & path, const char *filter, int flags)
{
    std::vector<FileInfo> fileInfo;
    getFilesInDir(path.c_str(), &fileInfo, filter, flags);
    int64_t sizeSum = 0;
    // Note: getFileInDir does not fill in fileSize properly.
    for (size_t i = 0; i < fileInfo.size(); i++)
    {
        FileInfo finfo;
        getFileInfo(fileInfo[i].fullName.c_str(), &finfo);
        if (!finfo.isDirectory)
        {
            sizeSum += finfo.size;
        }
        else
        {
            sizeSum += getDirectoryRecursiveSize(finfo.fullName, filter, flags);
        }
    }
    return sizeSum;
}

#ifdef WINDOWS
// Returns a vector with the device names
std::vector<std::string> getWindowsDrives()
{
    std::vector<std::string> drives;

    const DWORD buffsize = GetLogicalDriveStrings(0, NULL);
    std::vector<wchar_t> buff(buffsize);
    if (GetLogicalDriveStrings(buffsize, buff.data()) == buffsize - 1)
    {
        auto drive = buff.data();
        while (*drive)
        {
            std::string str(ConvertWStringToUTF8(drive));
            str.pop_back();    // we don't want the final backslash
            str += "/";
            drives.push_back(str);

            // advance to next drive
            while (*drive++) {}
        }
    }
    return drives;
}
#endif
