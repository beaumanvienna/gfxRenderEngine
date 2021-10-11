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

#include "file.h"

namespace EngineCore
{

    bool FileExists(const char* filename)
    {
        std::ifstream infile(filename);
        return infile.good();
    }

    bool FileExists(const std::string& filename)
    {
        std::ifstream infile(filename.c_str());
        return infile.good();
    }

    bool FileExists(const std::filesystem::directory_entry& filename)
    {
        return filename.exists();
    }

    bool IsDirectory(const char *filename)
    {
        std::filesystem::path path(filename);
        return is_directory(path);
    }

    bool IsDirectory(const std::string& filename)
    {
        std::filesystem::path path(filename);
        return is_directory(path);
    }

    std::string GetFilenameWithoutPath(const std::filesystem::path& path)
    {
        #ifndef _WIN32
            std::string filenameWithoutPath = path.filename();
        #else
            std::filesystem::path withoutPath{std::filesystem::path(path.filename())};
            std::string filenameWithoutPath = withoutPath.string();
        #endif
        return filenameWithoutPath;
    }

    std::string GetFileExtension(const std::filesystem::path& path)
    {
        #ifndef _WIN32
            std::string ext = path.extension();
        #else
            std::filesystem::path extension{std::filesystem::path(path.extension())}; 
            std::string ext = extension.string();
        #endif
        return ext;
    }

    bool CreateDirectory(const std::string& filename)
    {
        return std::filesystem::create_directories(filename);
    }

    bool CopyFile(const std::string& src, const std::string& dest)
    {
        std::ifstream source(src.c_str(), std::ios::binary);
        std::ofstream destination(dest.c_str(), std::ios::binary);
        destination << source.rdbuf();
        return source && destination;
    }

    std::ifstream::pos_type FileSize(const std::string& filename)
    {
        std::ifstream in(filename.c_str(), std::ifstream::ate | std::ifstream::binary);
        return in.tellg(); 
    }
}
