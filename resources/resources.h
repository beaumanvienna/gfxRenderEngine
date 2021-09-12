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

#include <memory>
#include <iostream>

#include "texture.h"
#include "windowsEmbeddedResources.h"

#ifndef _WIN32
    #include "linuxEmbeddedResources.h"
#else
    #include <cstddef>
    #include <string_view>
    #include <windows.h>
#endif

namespace ResourceSystem
{
    std::shared_ptr<Texture> GetTextureFromMemory(const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */);
    const void* GetDataPointer(size_t& fileSize, const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */);
    bool GetResourceString(std::string_view& destination, const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */);
}

#ifndef _WIN32

    namespace ResourceSystem
    {
        const void* GetDataPointer(std::size_t& fileSize, const char* path);
    }

#else
    
    class Resource 
    {
    public:
    
        Resource(int resourceID, const std::string &resourceClass) 
        {
            m_HResource = FindResourceA(nullptr, MAKEINTRESOURCEA(resourceID), resourceClass.c_str());
            m_HMemory   = LoadResource(nullptr, m_HResource);

            m_Parameters.m_SizeBytes = SizeofResource(nullptr, m_HResource);
            m_Parameters.m_DataPointer = LockResource(m_HMemory);
        }

        std::size_t GetSize() const { return m_Parameters.m_SizeBytes; }
        const void* GetDataPointer() const { return m_Parameters.m_DataPointer; }

    public:
    
        struct Parameters 
        {
            std::size_t m_SizeBytes = 0;
            void* m_DataPointer = nullptr;
        };

    private:
        HRSRC m_HResource = nullptr;
        HGLOBAL m_HMemory = nullptr;

        Parameters m_Parameters;
    };

    namespace ResourceSystem
    {
        const void* GetDataPointer(std::size_t& fileSize, int resourceID, const std::string& resourceClass);
    }

#endif
