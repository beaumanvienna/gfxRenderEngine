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
#include "scabb/rayTracing/hittable.h"

namespace ScabbApp
{

    glm::point3 Ray::At(float step) const
    {
        return m_Origin + step * m_Direction;
    }

    glm::color Ray::Color() const
    {
        auto t = HitSphere(glm::point3(0.0f,0.0f,-1.0f), 0.5f);
        if (t > 0.0)
        {
            glm::vec3 N = normalize(At(t) - glm::vec3(0.0f,0.0f,-1.0f));
            return 0.5f * glm::color(N.x+1, N.y+1, N.z+1);
        }
        glm::vec3 unitDirection = normalize(m_Direction);
        t = 0.5f * (unitDirection.y + 1.0f);
        return (1.0f-t) * glm::color(1.0f, 1.0f, 1.0f) + t * glm::color(0.5f, 0.7f, 1.0f);
    }

    float Ray::HitSphere(const glm::point3& center, float radius) const
    {
        glm::vec3 oc = m_Origin - center;

        auto a = glm::length2(m_Direction);
        auto half_b = glm::dot(oc, m_Direction);
        auto c = glm::length2(oc) - radius * radius;
        auto discriminant = half_b*half_b - a*c;

        if (discriminant < 0.0f)
        {
            return -1.0f;
        }
        else
        {
            
            return (-half_b - std::sqrt(discriminant)) / a;
        }
    }
}
