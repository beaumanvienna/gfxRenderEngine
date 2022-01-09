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

#include "glm.hpp"
#include "gtx/norm.hpp"
#include "gtx/compatibility.hpp"
#include "gtc/matrix_transform.hpp"
#include "engine.h"

namespace glm
{
    using point3 = vec3;
    using color  = vec3;
}

namespace ScabbApp
{
    // Constants
    #undef INFINITY
    constexpr float ASPECT_RATIO = 16.0f / 9.0f;
    constexpr float INFINITY = std::numeric_limits<float>::infinity();
    constexpr float PI = 3.1415926535897932385;
    
    // image
    constexpr uint IMAGE_HEIGHT = 512;
    constexpr uint IMAGE_WIDTH = static_cast<uint>(IMAGE_HEIGHT * ASPECT_RATIO);
    constexpr uint SAMPLES_PER_PIXEL = 100;
    constexpr float INV_SAMPLES_PER_PIXEL = 1.0f / SAMPLES_PER_PIXEL;
    
    // ray tracing
    constexpr uint BOUNCE_LIMIT = 10;
}
