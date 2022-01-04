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

#include "scabb/rayTracing/ray.h"
#include "scabb/rayTracing/aux.h"
#include "gtx/compatibility.hpp"

namespace ScabbApp
{

    glm::point3 Ray::At(float step) const
    {
        return m_Origin + step * m_Direction;
    }

    glm::color Ray::Color() const
    {
        auto circleBlendFactor = HitSphere(glm::point3(0.0f,0.0f,-1.0f), 0.5f);
        auto circleColor = glm::color(0.2f, 0.1f, 0.9f);

        glm::vec3 unitDirection = glm::normalize(m_Direction);

        // y normalized to -1, 1 --> step 0 to 1
        auto step = 0.5f*(unitDirection.y + 1.0f);
        auto white = glm::color(1.0f, 1.0f, 1.0f);
        auto lightblue = glm::color(0.5f, 0.7f, 1.0f);
        auto backgroundColor = glm::lerp(white, lightblue, step);

        return glm::lerp(backgroundColor, circleColor, circleBlendFactor);
    }

    float Ray::HitSphere(const glm::point3& center, float radius) const
    {
        glm::vec3 oc = m_Origin - center;
        auto a = glm::dot(m_Direction, m_Direction);
        auto b = 2.0f * glm::dot(oc, m_Direction);
        auto c = glm::dot(oc, oc) - radius*radius;
        auto discriminant = b*b - 4*a*c;
        float edge = 0.02f;
        return glm::smoothstep(-edge, edge, discriminant);
    }
}
