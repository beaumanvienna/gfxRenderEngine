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

#include <random>

#include "scabb/rayTracing/global.h"

namespace ScabbApp
{
    void LogColor(glm::color pixelColor);
    glm::vec3 RandomInUnitSphere();
    glm::vec3 RandomUnitVector();
    bool NearZero(const glm::vec3 vector);
}

inline float degreesToRadians(float degrees)
{
    return degrees * ScabbApp::PI / 180.0f;
}

// Returns a random real in [0.0f,0.0f).
inline float RandomFloat()
{
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

// Returns a random real in [min,max).
inline float RandomFloat(float min, float max)
{
    return min + (max-min)*RandomFloat();
}

inline static glm::vec3 Random()
{
    return glm::vec3(RandomFloat(), RandomFloat(), RandomFloat());
}

inline static glm::vec3 Random(float min, float max)
{
    return glm::vec3(RandomFloat(min,max), RandomFloat(min,max), RandomFloat(min,max));
}


inline float Clamp(float x, float min, float max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
