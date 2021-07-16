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

#include <cstring>

#include "path.h"
#include "utf8.h"
#include "stringUtils.h"

Path::Path(const std::string &str)
{
    if (str.empty())
    {
        type_ = PathType::UNDEFINED;
    }
    else if (startsWith(str, "http://") || startsWith(str, "https://"))
    {
        type_ = PathType::HTTP;
    }
    else
    {
        type_ = PathType::NATIVE;
    }

    Init(str);
}

#ifdef WINDOWS
Path::Path(const std::wstring &str)
{
    type_ = PathType::NATIVE;
    Init(ConvertWStringToUTF8(str));
}
#endif

void Path::Init(const std::string &str)
{
    path_ = str;

#ifdef WINDOWS
    for (size_t i = 0; i < path_.size(); i++)
    {
        if (path_[i] == '\\')
        {
            path_[i] = '/';
        }
    }
#endif

    if (type_ == PathType::NATIVE && path_.size() > 1 && path_.back() == '/')
    {
        path_.pop_back();
    }
}

Path Path::operator /(const std::string &subdir) const
{    

    if (subdir.empty())
    {
        return Path(path_);
    }
    std::string fullPath = path_;
    if (subdir.front() != '/')
    {
        fullPath += "/";
    }
    fullPath += subdir;
    
    if (fullPath.back() == '/')
    {
        fullPath.pop_back();
    }
    return Path(fullPath);
}

void Path::operator /=(const std::string &subdir)
{
    *this = *this / subdir;
}

Path Path::WithExtraExtension(const std::string &ext) const
{
    return Path(path_ + ext);
}

Path Path::WithReplacedExtension(const std::string &oldExtension, const std::string &newExtension) const
{    
    if (endsWithNoCase(path_, oldExtension))
    {
        std::string newPath = path_.substr(0, path_.size() - oldExtension.size());
        return Path(newPath + newExtension);
    }
    else
    {
        return Path(*this);
    }
}

Path Path::WithReplacedExtension(const std::string &newExtension) const
{
    
    if (path_.empty())
    {
        return Path(*this);
    }
    std::string extension = GetFileExtension();
    std::string newPath = path_.substr(0, path_.size() - extension.size()) + newExtension;
    return Path(newPath);
}

std::string Path::GetFilename() const
{
    
    size_t pos = path_.rfind('/');
    if (pos != std::string::npos)
    {
        return path_.substr(pos + 1);
    }
    return path_;
}

static std::string GetExtFromString(const std::string &str)
{
    size_t pos = str.rfind(".");
    if (pos == std::string::npos)
    {
        return "";
    }
    size_t slash_pos = str.rfind("/");
    if (slash_pos != std::string::npos && slash_pos > pos)
    {
        return "";
    }
    std::string ext = str.substr(pos);
    for (size_t i = 0; i < ext.size(); i++)
    {
        ext[i] = tolower(ext[i]);
    }
    return ext;
}

std::string Path::GetFileExtension() const
{
    
    return GetExtFromString(path_);
}

std::string Path::GetDirectory() const
{
    size_t pos = path_.rfind('/');
    if (type_ == PathType::HTTP)
    {
        if (pos + 1 == path_.size())
        {
            pos = path_.rfind('/', pos - 1);
            if (pos != path_.npos && pos > 8)
            {
                return path_.substr(0, pos + 1);
            }
        }
    }

    if (pos != std::string::npos)
    {
        if (pos == 0)
        {
            return "/";
        }
        return path_.substr(0, pos);
#ifdef WINDOWS
    } 
    else if (path_.size() == 2 && path_[1] == ':')
    {
        return "/";
#endif
    }
    else
    {
        size_t c_pos = path_.rfind(':');
        if (c_pos != std::string::npos)
        {
            return path_.substr(0, c_pos + 1);
        }
    }
    return path_;
}

bool Path::FilePathContains(const std::string &needle) const
{
    std::string haystack;
    if (type_ == PathType::CONTENT_URI)
    {
        
    }
    else
    {
        haystack = path_;
    }
    return haystack.find(needle) != std::string::npos;
}

bool Path::StartsWith(const Path &other) const
{
    if (type_ != other.type_)
    {    
        return false;
    }
    return startsWith(path_, other.path_);
}

const std::string &Path::ToString() const
{
    return path_;
}

#ifdef WINDOWS
std::wstring Path::ToWString() const
{
    std::wstring w = ConvertUTF8ToWString(path_);
    for (size_t i = 0; i < w.size(); i++)
    {
        if (w[i] == '/')
        {
            w[i] = '\\';
        }
    }
    return w;
}
#endif

std::string Path::ToVisualString() const
{
    
    return path_;
}

bool Path::CanNavigateUp() const
{
    if (path_ == "/" || path_ == "")
    {
        return false;
    }
    if (type_ == PathType::HTTP)
    {
        size_t rootSlash = path_.find_first_of('/', strlen("https://"));
        if (rootSlash == path_.npos || path_.size() < rootSlash + 1)
        {
            return false;
        }
    }
    return true;
}

Path Path::NavigateUp() const
{
    
    std::string dir = GetDirectory();
    return Path(dir);
}

Path Path::GetRootVolume() const
{
    if (!IsAbsolute())
    {
        
        return Path(path_);
    }

#ifdef WINDOWS
    if (path_[1] == ':')
    {
        std::string path = path_.substr(0, 2);
        return Path(path);
    }
#endif

    return Path("/");
}

bool Path::IsAbsolute() const
{
    if (type_ == PathType::CONTENT_URI)
    {
        return true;
    }

    if (path_.empty())
    {
        return true;
    }
    else if (path_.front() == '/')
    {
        return true;
    }
#ifdef WINDOWS
    else if (path_.size() > 3 && path_[1] == ':')
    {
        return true;
    }
#endif
    else
    {
        return false;
    }
}

std::string Path::PathTo(const Path &other)
{
    if (!other.StartsWith(*this))
    {
        return std::string();
    }

    std::string diff;

    if (path_ == "/")
    {
        diff = other.path_.substr(1);
    }
    else
    {
        diff = other.path_.substr(path_.size() + 1);
    }

    return diff;
}
