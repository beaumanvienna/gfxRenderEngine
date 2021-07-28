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

#include "resources.h"

#ifndef WINDOWS

    namespace ResourceSystem
    {
        const void* GetDataPointer(std::size_t& fileSize, const char* path)
        {
            GBytes* mem_access = g_resource_lookup_data(linuxEmbeddedResources_get_resource(), path, G_RESOURCE_LOOKUP_FLAGS_NONE, nullptr);
            return g_bytes_get_data(mem_access, &fileSize);
        }
    }

#else

    namespace ResourceSystem
    {
        const void* GetDataPointer(std::size_t& fileSize, int resourceID, const std::string& resourceClass)
        {
            Resource atlas(resourceID, resourceClass);
            fileSize = atlas.GetSize();
            return atlas.GetDataPointer();
        }
    }

#endif
